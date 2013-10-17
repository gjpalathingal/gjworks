#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_PHILOSOPHERS 5
#define NUM_CHOPSTICKS NUM_PHILOSOPHERS
#define NUM_ROUNDS 5

void philosopher_fn(void*);
pthread_mutex_t mutex_arr[NUM_CHOPSTICKS];
sem_t four_chairs;

int main()
{
    int i=0;
    pthread_t threads[5];

    for(i=0;i<NUM_CHOPSTICKS;i++){
            pthread_mutex_init(&mutex_arr[i],NULL);
//       mutex_arr[i] = PTHREAD_MUTEX_INITIALIZER;
    }
    sem_init(&four_chairs,0,4);
    i = 0;
    while(i<NUM_PHILOSOPHERS){
        if(pthread_create(&threads[i],NULL,philosopher_fn,(void*)i) != 0){
            perror("Error creating threads\n");
            return 1;
        }
        printf("Thread %d spawn\n",i);
        i++;
    }

    for(i=0;i<NUM_PHILOSOPHERS;i++){
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
    sem_wait(&four_chairs);
    printf("Philosopher %d occupied a chair [%d]\n",i,count);
    pthread_mutex_lock(&mutex_arr[i]);
    pthread_mutex_lock(&mutex_arr[right]);
    printf("Philosopher %d locked chopsticks [%d]\n",i,count);
    printf("Philosopher %d going to eat [%d]\n",i,count);
    eat();
    printf("Philosopher %d going to unlock chopsticks [%d]\n",i,count);
    pthread_mutex_unlock(&mutex_arr[right]);
    pthread_mutex_unlock(&mutex_arr[i]);
    sem_post(&four_chairs);
    printf("Philosopher %d gave up a chair [%d]\n",i,count);
    }
}

void think(){
    sleep(1);
}

void eat(){
    sleep(1);
}
