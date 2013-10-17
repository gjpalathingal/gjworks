#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_PHILOSOPHERS 5
#define NUM_CHOPSTICKS NUM_PHILOSOPHERS
#define NUM_ROUNDS 5

void philosopher_fn(void*);
pthread_mutex_t mutex_arr[5];

int main()
{
    int i=0;
    pthread_t threads[5];

    for(i=0;i<5;i++){
            pthread_mutex_init(&mutex_arr[i],NULL);
//       mutex_arr[i] = PTHREAD_MUTEX_INITIALIZER;
    }
    i = 0;
    while(i<5){
        if(pthread_create(&threads[i],NULL,philosopher_fn,(void*)i) != 0){
            perror("Error creating threads\n");
            return 1;
        }
        printf("Thread %d spawn\n",i);
        i++;
    }

    for(i=0;i<5;i++){
       pthread_join(threads[i],NULL);
    }
    return 0;
}

void philosopher_fn(void* no){
    int i = (int)no;
    int right,count;
    right = (i+1)%NUM_CHOPSTICKS;
    for(count=0;count<NUM_ROUNDS;count++){
    printf("Philosopher %d going to think [%d]\n",i,count);
    think();
    pthread_mutex_lock(&mutex_arr[i]);
    pthread_mutex_lock(&mutex_arr[right]);
    printf("Philosopher %d locked chopsticks [%d]\n",i,count);
    printf("Philosopher %d going to eat [%d]\n",i,count);
    eat();
    printf("Philosopher %d going to unlock chopsticks [%d]\n",i,count);
    pthread_mutex_unlock(&mutex_arr[right]);
    pthread_mutex_unlock(&mutex_arr[i]);
    }
}

void think(){
    sleep(1);
}

void eat(){
    sleep(1);
}

