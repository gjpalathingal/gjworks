/*
 * duplicate_removal_method_1.c
 *
 *  Created on: Sep 27, 2013
 *      Author: George
 */
#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>

//#define METHOD1 //Normal value compare using 2 arrays
#define METHOD2 //Using mergesort




int merge(int *arr, int *temp, int left, int mid, int right){
	int left_end = mid-1;
	int i,dup = 0;
	int temp_pos = left;
	int num_elements = right - left + 1;
	while((left<=left_end) && (mid <= right)){
		if(arr[left] == arr[mid]){
			temp[temp_pos] = arr[left];
			temp_pos++;
			left++;
			mid++;
			dup++;
		}
		else if (arr[left] < arr[mid]){
			temp[temp_pos] = arr[left];
			temp_pos++;
			left++;
		}
		else{
			temp[temp_pos] = arr[mid];
			temp_pos++;
			mid++;
		}

	}
	while(left <= left_end){
		temp[temp_pos] = arr[left];
		temp_pos++;
		left++;
	}
	while(mid<=right){
		temp[temp_pos] = arr[mid];
		temp_pos++;
		mid++;
	}
	for(i=0;i<num_elements;i++){
		arr[right] = temp[right];
		right--;
	}
	return dup;
}

void msort(int *arr,int *temp, int left, int right){
	int mid, dup = 0, i;
	if(right > left){
		mid = (right+left)/2;
		msort(arr,temp, left, mid);
		msort(arr,temp, mid+1, right);
		dup+= merge(arr, temp, left, mid+1, right);
	}
	for(i=right;dup>0;i--,dup--)
		arr[i] = 0;
}

void dupless_mergesort(int *arr,int length){
	int left = 0, right = length-1;
	int *temp = (int*)calloc(length,sizeof(int));
	msort(arr,temp,left,right);
	return;
}

int check_duplicate(int* a, int num,int count){
	int i,flag = 0;
	for(i=0;i<=count;i++){
		if(*(a+i) == num)
		 flag++;
	}
	return flag;
}

int* rm_duplicate(int *arr){
	int i,j;
	int* temp = (int*)calloc(15,sizeof(int));
	*temp = arr[0];
	for(i=1,j=1;i<15;){
		if(check_duplicate(temp,arr[i],j)){
			i++;
			continue;
		}
		*(temp+j) = arr[i];
		i++;
		j++;
	}
	return temp;
}
int main()
{
	int i,len;
	int *arr = NULL;
	int *new_arr = NULL;
	FILE *fin = fopen("input.txt","r");
	FILE *fout = fopen("output.txt","w");
	fscanf(fin,"%d\n",&len);
	printf("The number of elements for duplicate elimination: %d\n",len);
	arr = (int*)calloc(len,sizeof(int));
	for(i=0;i<len;i++)
	{
		fscanf(fin,"%d",arr+i);
		printf("%d ",*(arr+i));
	}
#ifdef METHOD1
	new_arr = rm_duplicate(arr);
#else
#ifdef	METHOD2
	dupless_mergesort(arr,len);

#else
	new_arr = rm_dup_hash(arr,len);
#endif
#endif
	for(i=0 ;i<len; i++){
		fprintf(fout,"%d ",*(arr+i));
	}
	return 0;
}

