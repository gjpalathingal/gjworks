#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct mod_arr{
    int *arr;
    int size;
};

struct arr_merge{
    struct mod_arr* arr1;
    struct mod_arr* arr2;
    struct mod_arr* arr3;
    int count;
};

pthread_mutex_t mutex_var = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

void threadfunc(void*);

int main()
{
    int i;
    pthread_t* threads;
    struct arr_merge *arr_obj;
    arr_obj = (struct arr_merge*)malloc(sizeof(struct arr_merge));
    arr_obj->arr1 = (struct mod_arr*)malloc(sizeof(struct mod_arr));
    arr_obj->arr2 = (struct mod_arr*)malloc(sizeof(struct mod_arr));
    arr_obj->arr3 = (struct mod_arr*)malloc(sizeof(struct mod_arr));
    printf("Enter the number of elements for array1:");
    scanf("%d",&(arr_obj->arr1->size));
    arr_obj->arr1->arr = (int*)malloc(sizeof(int)* (arr_obj->arr1->size));
    printf("Enter the elements of array1: ");
    for(i=0;i<arr_obj->arr1->size;i++){
        scanf("%d",&(arr_obj->arr1->arr[i]));
    }
    printf("Enter the number of elements for array2:");
    scanf("%d",&(arr_obj->arr2->size));
    arr_obj->arr2->arr = (int*)malloc(sizeof(int)* (arr_obj->arr2->size));
    printf("Enter the elements of array2: ");
    for(i=0;i<arr_obj->arr2->size;i++){
        scanf("%d",&(arr_obj->arr2->arr[i]));
    }

    arr_obj->arr3->size = arr_obj->arr1->size + arr_obj->arr2->size;
    arr_obj->arr3->arr = (int*)malloc(sizeof(int)*arr_obj->arr3->size);
    arr_obj->count = 0;

   threads = (pthread_t*)malloc(arr_obj->arr3->size*sizeof(pthread_t*));

   for(i=0;i<arr_obj->arr3->size;i++){
    if(pthread_create(&threads[i],NULL,threadfunc,(void*)arr_obj) != 0){
        perror("Thread creation failed\n");
        return 1;
        }
   pthread_mutex_lock(&mutex_var);
   pthread_cond_wait(&cond_var, &mutex_var);
   arr_obj->count++;
   pthread_mutex_unlock(&mutex_var);
    }

    for(i=0;i<arr_obj->arr3->size;i++){
        pthread_join(threads[i],NULL);
    }

    printf("The new array is as follows\n");
    for(i=0;i<arr_obj->arr3->size;i++){
        printf("%d ",arr_obj->arr3->arr[i]);
    }
    printf("\n");
    free(threads);
    free(arr_obj->arr1->arr);
    free(arr_obj->arr1);
    free(arr_obj->arr2->arr);
    free(arr_obj->arr2);
    free(arr_obj->arr3->arr);
    free(arr_obj->arr3);
    free(arr_obj);
   return 0;
}


void threadfunc(void* arrobj){
    struct arr_merge* temp_arrobj;
    int lower, upper,pos, middle,i,cnt,temp_cnt;
    struct mod_arr *temp_arr1, *temp_arr2;
    temp_arrobj = (struct arr_merge*)arrobj;
    pthread_mutex_lock(&mutex_var);
    temp_cnt = temp_arrobj->count;
    pthread_cond_signal(&cond_var);
    pthread_mutex_unlock(&mutex_var);

    if(temp_cnt < temp_arrobj->arr1->size){
            temp_arr1 = temp_arrobj->arr1;
            temp_arr2 = temp_arrobj->arr2;
            cnt = temp_cnt;
            }else{
            temp_arr1 = temp_arrobj->arr2;
            temp_arr2 = temp_arrobj->arr1;
            cnt = temp_cnt - temp_arrobj->arr1->size;
            }
    printf("Thread %d Count: %d\n",temp_cnt,cnt);
    if(*(temp_arr1->arr+cnt) < *(temp_arr2->arr)){
        printf("Thread %d: Case1\n",temp_cnt);
        pos = cnt;
        printf("Thread %d: Position: %d\n",temp_cnt, pos);
    }
    else if(*(temp_arr1->arr+cnt) > *(temp_arr2->arr+(temp_arr2->size)-1)){
        printf("Thread %d: Case2\n",temp_cnt);
        pos = temp_arr2->size+cnt;
        printf("Thread %d: Position: %d\n",temp_cnt, pos);
    }
    else{
        printf("Thread %d: Case3\n",temp_cnt);
        lower = 0;
        upper =  temp_arr2->size - 1;
        while(upper > lower){
            middle = (upper + lower)/2;
            if(*(temp_arr1->arr+cnt) > *(temp_arr2->arr+middle) && *(temp_arr1->arr+cnt) < *(temp_arr2->arr+middle+1)){
                break;
            }
            else if(*(temp_arr1->arr+cnt) < *(temp_arr2->arr+middle))
                upper = middle;
            else if(*(temp_arr1->arr+cnt) > *(temp_arr2->arr+middle))
                lower = middle + 1;
        }
        pos = cnt + middle + 1;
        printf("Thread %d: Position: %d\n",temp_cnt, pos);
    }
    temp_arrobj->arr3->arr[pos] = *(temp_arr1->arr+cnt);
}

