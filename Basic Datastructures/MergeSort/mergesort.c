/*
 * mergesort.c
 *
 *  Created on: Sep 27, 2013
 *      Author: George
 */

#include<stdlib.h>
#include<stdio.h>

void merge(int *numbers, int* temp, int left, int mid, int right){
	int num_elements, left_end,tmp_pos,i;
	printf("Merge on(%d %d %d)\n",left,mid,right);
	left_end = mid-1;
	tmp_pos = left;
	num_elements = right -left + 1;

	while((left<=left_end) && (mid<=right)){
		if(numbers[left] < numbers[mid]){
			temp[tmp_pos] = numbers[left];
			tmp_pos+=1;
			left+=1;
		}
		else{
			temp[tmp_pos] = numbers[mid];
			mid+=1;
			tmp_pos+=1;
		}
	}
		while(left <= left_end){
			temp[tmp_pos] = numbers[left];
			tmp_pos+=1;
			left+=1;
		}
		while(mid <= right){
			temp[tmp_pos] = numbers[mid];
			mid+=1;
			tmp_pos+=1;
		}
	  for (i=0; i <= num_elements; i++)
	  {
	    numbers[right] = temp[right];
	    right = right - 1;
	  }
}

void m_sort(int* numbers, int* temp, int left, int right){
	int mid;

	if(right > left){
	mid = (right + left)/2;
	m_sort(numbers, temp, left, mid);
	m_sort(numbers, temp, mid+1, right);

	merge(numbers, temp, left, mid+1, right);
	}
}

void mergeSort(int *numbers, int* temp, int len){
	m_sort(numbers, temp, 0, len-1);
}

int main()
{
	int len,i,*numbers,*temp;
	FILE *fin = fopen("input.txt","r");
	FILE *fout = fopen("output.txt","w");
	fscanf(fin,"%d\n",&len);
	printf("The number of numbers to be sorted: %d\n",len);
	numbers = (int*)malloc(sizeof(int)*len);
	temp = (int*)malloc(sizeof(int)*len);

	for(i=0;i<len;i++){
		fscanf(fin,"%d",numbers+i);
		printf("Enter the numbers to be sorted: %d\n",numbers[i]);
	}
	mergeSort(numbers, temp, len);
	for(i=0;i<len;i++)
		fprintf(fout,"%d ",*(temp+i));
	fclose(fin);
	fclose(fout);
	return 0;
}


