// Insertion_sort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdlib.h>


int _tmain(int argc, _TCHAR* argv[])
{
	int size,i,j;
	int *arr,*key;
	printf("Enter the number of elements to be sorted\n");
	scanf_s("%d",&size);
	arr=(int*)malloc(sizeof(int)*size);
	printf("Enter the elements: ");
	for(i=0;i<size;i++)
		scanf_s("%d",arr+i);
	printf("\n");
	for(i=1;i<size;i++){
		key = arr+i;
		while(key > arr && *(key-1) > *key){
		j = *key;
		*key = *(key-1);
		*(key-1) = j;
		key--;
	}
}
	printf("Sorted array in ascending order :");
	for(i=0;i<size;i++) printf("%d",*(arr+i));
	getchar();
	return 0;
}

