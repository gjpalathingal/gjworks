#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define SMOKERS 3
#define AGENT 1
#define ROUNDS 5

sem_t tob_paper;
sem_t match_paper;
sem_t tob_match;

pthread_mutex_t table_mutex;
pthread_cond_t condition_var;

void serve_tob_paper();
void serve_match_paper();
void serve_tob_match();

void consumer_tob_paper();
void consumer_match_paper();
void consumer_tob_match();

void smoker_fn(void*);
void agent_fn();

int tobacco = 0,match = 0, paper = 0;
int main()
{
    int i = 0;
    pthread_t smoker_thread[SMOKERS],agent_thread;
    pthread_mutex_init(&table_mutex,NULL);
    pthread_cond_init(&condition_var,NULL);
    sem_init(&tob_paper, 0 , 0);
    sem_init(&match_paper, 0 , 0);
    sem_init(&tob_match, 0 , 0);
    if(pthread_create(&agent_thread,NULL,agent_fn,NULL) != 0){
            perror("Agent thread can't be created\n");
            return 1;
       }
    for(i=0;i<SMOKERS;i++){
        if(pthread_create(&smoker_thread[i],NULL,smoker_fn,(void*)i) != 0){
            perror("Smoker threads can't be created\n");
            return 2;
        }
    }
    pthread_join(agent_thread,NULL);
    for(i=0;i<SMOKERS;i++){
        pthread_join(smoker_thread[i],NULL);
    }
    return 0;
}


void agent_fn(){
    int i;
    for(i=0;i<ROUNDS;i++){
    printf("----------------------ROUND %d-----------------------\n",i);
    pthread_mutex_lock(&table_mutex);
    serve_tob_paper();
 //   sem_post(&tob_paper);
    printf("Tobacco & Paper are kept on table\n");
    sem_post(&tob_paper);
    pthread_cond_wait(&condition_var, &table_mutex);
    serve_match_paper();
    printf("Match & Paper are kept on table\n");
    sem_post(&match_paper);
    pthread_cond_wait(&condition_var, &table_mutex);
    serve_tob_match();
    printf("Tobacco & Match are kept on table\n");
    sem_post(&tob_match);
    pthread_cond_wait(&condition_var, &table_mutex);
    pthread_mutex_unlock(&table_mutex);
    }
}

void smoker_fn(void* index)
{
    int num,i;
    num = (int)index;
    for(i=0;i<ROUNDS;i++){
    switch(num){
    case 0:
        printf("[Round %d][Smoker %d]Waiting...\n",i,num);
        sem_wait(&tob_paper);
        printf("[Smoker %d]Got signal for tob_paper\n",num);
        pthread_mutex_lock(&table_mutex);
        printf("[Smoker %d]Locked table for tob_paper\n",num);
        consume_tob_paper();
        printf("[Smoker %d]Consumed tob_paper\n",num);
        pthread_cond_signal(&condition_var);
        printf("[Smoker %d]Signalled Agent\n",num);
        pthread_mutex_unlock(&table_mutex);
        break;
    case 1:
        printf("[Round %d][Smoker %d]Waiting...\n",i,num);
        sem_wait(&match_paper);
        printf("[Smoker %d]Got signal for match_paper\n",num);
        pthread_mutex_lock(&table_mutex);
        printf("[Smoker %d]Locked table for match_paper\n",num);
        consume_match_paper();
        printf("[Smoker %d]Consumed match_paper\n",num);
        pthread_cond_signal(&condition_var);
        printf("[Smoker %d]Signalled Agent\n",num);
        pthread_mutex_unlock(&table_mutex);
        break;
    case 2:
        printf("[Round %d][Smoker %d]Waiting...\n",i,num);
        sem_wait(&tob_match);
        printf("[Smoker %d]Got signal for tob_match\n",num);
        pthread_mutex_lock(&table_mutex);
        printf("[Smoker %d]Locked table for tob_match\n",num);
        consume_tob_match();
        printf("[Smoker %d]Consumed tob_match\n",num);
        pthread_cond_signal(&condition_var);
        printf("[Smoker %d]Signalled Agent\n",num);
        pthread_mutex_unlock(&table_mutex);
        break;
        }
    }
}

void serve_tob_paper(){
    tobacco++;
    paper++;
}

void serve_match_paper(){
    match++;
    paper++;
}

void serve_tob_match(){
    tobacco++;
    match++;
}

void consume_tob_paper(){
    tobacco--;
    paper--;
}

void consume_match_paper(){
    match--;
    paper--;
}

void consume_tob_match(){
    tobacco--;
    match--;
}
