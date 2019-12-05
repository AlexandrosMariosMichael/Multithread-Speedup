#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
//–lpthread
pthread_mutex_t mutex;

struct arg_struct{
    int *A,*B,*N;
    int m,n,l;
    int nt,counter;
    long long sum;
};

void table_fills(int *A,int *B,int *N,int m, int n,int l);
void* rec_sum(void* arg);
double gettime(void);

int main()
{
    struct arg_struct args;

    int m,n,l,nt,i,k;
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

    args.A=A;
    args.B=B;
    args.N=N;

    args.m=m;
    args.n=n;
    args.l=l;
    args.sum=0;

    args.nt=nt;
    args.counter=0;

    pthread_mutex_init(&mutex,NULL);
    pthread_t tids[nt];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    double time0 = gettime();
    
  
        
    for ( i = 0; i < nt; ++i)
    {   
        pthread_create(&tids[i], &attr,rec_sum,&args);      
        pthread_join(tids[i], NULL);  
        args.counter=i;    
   
    }

  
    
    printf("\nThe sum is: %lld\n",args.sum );
     double time1 = gettime();
       printf("Calculating Time of Hamming Distance in seconds (pthreads a): %f\n",time1-time0);
    free(A);
    free(B);
    free(N);

    

return 0;
}


void* rec_sum(void* arg)
{
    int dist=0;
    int k,i,j;
    int sm,sn,sl,em,en,el,nt,counter,offset1,offset2,offset3;
    int *A,*B,*N;
    struct  arg_struct *args_s= (struct arg_struct*) arg;
  
    nt=args_s->nt;
    offset1= (args_s->m / nt);
    offset2=(args_s->n / nt);
    offset3=(args_s->l / nt);
    counter= args_s->counter;
    A=args_s->A;
    B=args_s->B;
    N=args_s->N;

    sm=counter *offset1;
    em=(counter+1)*offset1;
  
    sn=counter* offset2;
    en=(counter+1)*offset2;

    sl=counter* offset3;
    el=(counter+1)*offset3;


     for ( k = sm; k <em; ++k)
    {
        for ( i = sn; i < en; ++i)
        {
           for ( j = sl; j < el; j++)
            {   
               //calculating Hammind distance
                if((*(A + k * el + j)) != (*(B + i * el + j )))
                {
               
                dist++;
                
               
                }
            }       
           
            pthread_mutex_lock(&mutex);
            *(N+k*en+i)=  dist;
            args_s->sum+= dist ;
            dist = 0;
            pthread_mutex_unlock(&mutex);
        }
    }


    pthread_exit((void*)0);
    
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