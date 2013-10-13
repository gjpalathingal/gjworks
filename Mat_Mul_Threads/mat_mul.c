#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 4

int num_threads;

int A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];

void init_matrix(int (*mat)[SIZE]){
    int i,j;
    int *temp;
    for(i=0;i<SIZE;i++){
            for(j=0;j<SIZE;j++){
            temp = mat[i]+j;
            *temp = i+j;
            }
    }
}

void print_matrix(int (*mat)[SIZE])
{
	 int i, j;
	 for (i = 0; i < SIZE; i++) {
		 printf("\n\t| ");
		 for (j = 0; j < SIZE; j++)
			 printf("%2d ", *(mat[i]+j));
		 printf("|");
	 }
}

void multiply(void* slice){
    int s,to,from,i,j,k;
    s = (int)slice;
    from = (s*SIZE)/num_threads;
    to = ((s+1)*SIZE)/num_threads;

    printf("computing slice %d (from row %d to %d)\n", s, from, to-1);

    for(i = from; i<to;i++){
        for(j = 0; j < SIZE; j++){
            C[i][j] = 0;
            for(k = 0; k < SIZE; k++)
                C[i][j]+= A[i][k]*B[k][j];
        }
    }

     printf("finished slice %d\n", s);
	 return;
}

int main(int argc, char* argv[]){
    pthread_t *threads;
    int i;
    if(argc!=2){
        printf("Usage %s Number_of_threads\n",argv[0]);
        return 1;
    }
    num_threads = atoi(argv[1]);
    threads = (pthread_t*)malloc(num_threads*sizeof(pthread_t));
    init_matrix(A);
    init_matrix(B);
    init_matrix(C);

 // this for loop not entered if thread number is specified as 1
    for(i=1;i<num_threads;i++){
		 // creates each thread working on its own slice of i
        if(pthread_create((threads+i),NULL, multiply, (void*)i) !=0){
            perror("Can't create threads\n");
            free(threads);
            exit(-1);
        }
    }
	 // main thread works on slice 0
	 // so everybody is busy
	 // main thread does everything if threadd number is specified as 1
       multiply(0);

    // main thead waiting for other threads to complete
       for(i=0;i<num_threads;i++)
            pthread_join(*(threads+i),NULL);

     printf("\n\n");
	 print_matrix(A);
	 printf("\n\n\t * \n");
	 print_matrix(B);
	 printf("\n\n\t = \n");
	 print_matrix(C);
	 printf("\n\n");

	 free(threads);
    return 0;
}
