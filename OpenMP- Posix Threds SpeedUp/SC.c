#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

void table_fills(int *A,int *B,int *N,int m, int n,int l);
int rec_sum(int *A, int *B, int *N,int m , int n, int l);
double gettime(void);

int main()
{
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
    puts("\nOk Calculating...\n");

    A = (int*)malloc(l*m*sizeof(int));
    B = (int*)malloc(l*n*sizeof(int));
    N = (int*)malloc(n*m*sizeof(int));
    table_fills(A,B,N,m,n,l);

    double time0 = gettime();

    sum=rec_sum(A,B,N,m,n,l);
  

    printf("\nThe sum is: %ld\n",sum );
   
    double time1 = gettime();

    printf("Calculating Time of Hamming Distance in seconds (Sequential): %f\n",time1-time0);
    
    free(A);
    free(B);
    free(N);

    

return 0;
}

int rec_sum(int *A, int *B, int *N,int m , int n, int l)
{
    int dist=0;
    int k,i,j;
    long int sum=0;
     for ( k = 0; k < m; ++k)
    {
        for ( i = 0; i < n; ++i)
        {

             for ( j = 0; j < l; j++)
            {
               
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
    return sum;
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