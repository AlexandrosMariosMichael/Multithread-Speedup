#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <mpi.h>

double gettime(void)
{
	struct timeval ttime;
	gettimeofday(&ttime , NULL);
	return ttime.tv_sec + ttime.tv_usec * 0.000001;
}
float randpval ()
{
	int vr = rand();
	int vm = rand()%vr;
	float r = ((float)vm)/(float)vr;
	assert(r>=0.0 && r<=1.00001);
	return r;
}

int main(int argc, char ** argv)
{
	int rank,proc_nums;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&proc_nums);//Total number of procs
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);//Rank of every proc 0,1...

	 
	

	int N = atoi(argv[1]);


	int iters = 1000;
	srand(1);
	float * mVec = (float*)_mm_malloc(sizeof(float)*N,16);
	assert(mVec!=NULL);
	float * nVec = (float*)_mm_malloc(sizeof(float)*N,16);
	assert(nVec!=NULL);
	float * LVec = (float*)_mm_malloc(sizeof(float)*N,16);
	assert(LVec!=NULL);
	float * RVec = (float*)_mm_malloc(sizeof(float)*N,16);
	assert(RVec!=NULL);
	float * CVec = (float*)_mm_malloc(sizeof(float)*N,16);
	assert(CVec!=NULL);
	float * FVec = (float*)_mm_malloc(sizeof(float)*N,16);
	assert(FVec!=NULL);

	__m128 * v1_mVec_ptr =(__m128*)mVec;

	__m128 * v2_nVec_ptr =(__m128*)nVec;

	__m128 * v3_LVec_ptr =(__m128*)LVec;

	__m128 * v4_RVec_ptr =(__m128*)RVec;

	__m128 * v5_CVec_ptr =(__m128*)CVec;

	__m128 * v6_FVec_ptr =(__m128*)FVec;

	

	for(int i=0;i<N;i++)
	{
		mVec[i] = (float)(2+rand()%10);
		nVec[i] = (float)(2+rand()%10);
		LVec[i] = 0.0;
		for(int j=0;j<mVec[i];j++)
		{
			LVec[i] += randpval();
		}
		RVec[i] = 0.0;
		for(int j=0;j<nVec[i];j++)
		{
			RVec[i] += randpval();
		}
		CVec[i] = 0.0;
		for(int j=0;j<mVec[i]*nVec[i];j++)
		{
			CVec[i] += randpval();
		}
		FVec[i] = 0.0;
		assert(mVec[i]>=2.0 && mVec[i]<=12.0);
		assert(nVec[i]>=2.0 && nVec[i]<=12.0);
		assert(LVec[i]>0.0 && LVec[i]<=1.0*mVec[i]);
		assert(RVec[i]>0.0 && RVec[i]<=1.0*nVec[i]);
		assert(CVec[i]>0.0 && CVec[i]<=1.0*mVec[i]*nVec[i]);
	}
		
		float maxF=0.0f;
		float max;
		float vect[4];

		float t= 0.01;
		float t1=1.0;
		float t2=2.0;
		__m128 temp2=_mm_set_ps1(t1);
		__m128 temp3=_mm_set_ps1(t2);
		__m128 temp1=_mm_set_ps1(t);

		double timeTotal = 0.0f;


		int offset=0;
		int chunksize=N/(4*proc_nums);
		MPI_Barrier(MPI_COMM_WORLD);
		double time0=gettime();

	if (rank==0)
	{
		
		offset=chunksize;

		int other_rank;
		for ( other_rank = 1; other_rank < proc_nums; other_rank++)
		{
			
			MPI_Send(&offset,1,MPI_INT,other_rank,0,MPI_COMM_WORLD);
			offset=offset+chunksize;

		}
			
			
		for(int j=0;j<iters;j++)
		{	
			for(int i=0;i< chunksize;i++)
			{

				__m128 num_0= _mm_add_ps(v3_LVec_ptr[i],v4_RVec_ptr[i]);
				__m128 num_1 = _mm_mul_ps(v1_mVec_ptr[i],((v1_mVec_ptr[i]-1.0)/2.0));
				__m128 num_2 = _mm_mul_ps(v2_nVec_ptr[i],((v2_nVec_ptr[i]-1.0)/2.0));

					//__m128	numtemp=_mm_add_ps(num_1,num_2);
			
				//__m128 num= _mm_div_ps(num_0,numtemp);
				__m128 num= num_0/(num_1+num_2);

				__m128	den_0temp = _mm_sub_ps(v5_CVec_ptr[i],v3_LVec_ptr[i]);
				__m128 den_0 = _mm_sub_ps(den_0temp,v4_RVec_ptr[i]);
				__m128 den_1 = _mm_mul_ps(v1_mVec_ptr[i],v2_nVec_ptr[i]);
				__m128 den=_mm_div_ps(den_0,den_1);				
				 //	__m128 temp=_mm_add_ps(den,temp1);
				//v6_FVec_ptr[i] = _mm_div_ps(num,temp);
				v6_FVec_ptr[i] = num/(den+temp1);

				_mm_storeu_ps(vect,v6_FVec_ptr[i]);
				for (int k=0; k<4; k++) 
				maxF = vect[k]>maxF?vect[k]:maxF;
				
			}			

			
		}

			
			

		for ( other_rank = 1; other_rank < proc_nums; other_rank++)
		{
			MPI_Recv(&offset,1,MPI_INT,other_rank,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
			
		}

		//printf("--%d---maxF %f\n",rank,maxF );

	MPI_Reduce(&maxF,&max,1,MPI_FLOAT,MPI_MAX,0,MPI_COMM_WORLD);
	//printf("SSE+MPI with N %d P= %d Time %f Max %f\n",N,proc_nums, timeTotal/iters, sum); //used to be timeTotal/iters



	}else if(rank >0 )
	{


		MPI_Recv(&offset,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
		int off=offset;
		//printf("--%d--star%d\n",rank,off );
		//printf("--%d--end%d\n",rank,off+chunksize );
		
		for(int j=0;j<iters;j++)
		{	
			for(int i=off;i < off + chunksize;i++)
			{


				__m128 num_0= _mm_add_ps(v3_LVec_ptr[i],v4_RVec_ptr[i]);
				__m128 num_1 = _mm_mul_ps(v1_mVec_ptr[i],((v1_mVec_ptr[i]-1.0)/2.0));
				__m128 num_2 = _mm_mul_ps(v2_nVec_ptr[i],((v2_nVec_ptr[i]-1.0)/2.0));
					//__m128	numtemp=_mm_add_ps(num_1,num_2);
				//__m128 num= _mm_div_ps(num_0,numtemp);
				__m128 num= num_0/(num_1+num_2);
				__m128	den_0temp = _mm_sub_ps(v5_CVec_ptr[i],v3_LVec_ptr[i]);
				__m128 den_0 = _mm_sub_ps(den_0temp,v4_RVec_ptr[i]);
				__m128 den_1 = _mm_mul_ps(v1_mVec_ptr[i],v2_nVec_ptr[i]);
				__m128 den=_mm_div_ps(den_0,den_1);				
				 //	__m128 temp=_mm_add_ps(den,temp1);
				//v6_FVec_ptr[i] = _mm_div_ps(num,temp);
				v6_FVec_ptr[i] = num/(den+temp1);

				_mm_storeu_ps(vect,v6_FVec_ptr[i]);
				for (int k=0; k<4; k++) 
				maxF = vect[k]>maxF?vect[k]:maxF;
			
						
			}

			

		}

					

		MPI_Send(&offset,1,MPI_INT,0,0,MPI_COMM_WORLD);
		//printf("--%d---maxF %f\n",rank,maxF );
		MPI_Reduce(&maxF,&max,1,MPI_FLOAT,MPI_MAX,0,MPI_COMM_WORLD);


	}

	double time1=gettime();
	timeTotal += time1-time0;
	if (rank==0)
	{
		printf("SSE+MPI with N %d P= %d Time %f Max %f\n",N,proc_nums, timeTotal, max); //used to be timeTotal/iters
	}

	_mm_free(mVec);
	_mm_free(nVec);
	_mm_free(LVec);
	_mm_free(RVec);
	_mm_free(CVec);
	_mm_free(FVec);

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	return(0);
	
}