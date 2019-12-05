#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>
void table_fills(int *A,int *B,int *N,int m, int n,int l);
double gettime(void);

int main()
{
    
    int k,i,j;
    long int sum=0;
    int m,n,l,nt;
    int dist=0;
    int *A=NULL,*B=NULL,*N=NULL;

    time_t t;
    srand((unsigned) time(&t));

    puts("\nGive me #A");
    scanf("%d",&m);
    puts("Give me #B");
    scanf("%d",&n);
    puts("Give me Length");
    scanf("%d",&l);
    puts("Give me num of Threads");
    scanf("%d",&nt);
    puts("\nOk Calculating...\n");
   
    A = (int*)malloc(l*m*sizeof(int));
    B = (int*)malloc(l*n*sizeof(int));
    N = (int*)malloc(n*m*sizeof(int));

    table_fills(A,B,N,m,n,l);

    omp_set_num_threads(nt);
    
    
    double time0 = gettime();


    #pragma omp parallel shared(A, B , N) private(k, i, j,dist)
    {

    
    #pragma omp for reduction (+:sum)

     for ( k = 0; k < m; ++k)
    {
        
        //#pragma omp for reduction (+:sum)

        for ( i = 0; i < n; ++i)
        {
            // #pragma omp for reduction (+:sum)

             for ( j = 0; j < l; j++)
            {
                // #pragma omp  reduction (+:sum)

               //calculating Hammind distance
                if((*(A+k*l+j)) != (*(B+i*l+j)))
                {
                    
                dist++;
                }
            }
           

            *(N+k*n+i)=  dist;
            sum=  sum+ dist ;
            dist = 0;
    
        }
        

    }
}

     double time1 = gettime();
     
    printf("\nThe sum is: %ld\n",sum );
    printf("Calculating Time of Hamming Distance in seconds (OpenMPa): %f\n",time1-time0);
 
    free(A);
    free(B);
    free(N);

    

return 0;
}


void table_fills(int *A, int *B, int *N,int m , int n, int l)
{
    int i,j;
   
    


    if (m==n)
    {
        
        for ( i = 0; i < m; ++i)
        {
            for ( j=0; j<l; j++)
            {
                *(A+i*l+j) = (int)rand() % 2;
                *(B+i*l+j) = (int)rand() % 2;
            }
        }


    }
    else
    {
        
        for (i=0; i<m; i++)
        for ( j=0; j<l; j++)
           *(A+i*l+j) =(int)rand() % 2;

        for (i=0; i<n; i++)
        for ( j=0; j<l; j++)
          *(B+i*l+j) = (int)rand() % 2;
   }
}

double gettime(void)

{
struct timeval ttime;
gettimeofday(&ttime , NULL);
return ttime.tv_sec + ttime.tv_usec * 0.000001;
}