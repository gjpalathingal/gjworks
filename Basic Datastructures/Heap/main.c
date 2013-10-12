#include <stdio.h>
#include <stdlib.h>

#define MAX 10

void max_heapify(int *, int, int);
void build_max_heap(int*, int);
void heapsort(int*,int);
//void extract_max();

int main()
{
	int i,a[MAX],n;
	printf("Enter the number of elements: ");
	scanf("%d",&n);

	printf("Enter the elements: ");
	for(i=0;i<n;i++)
	scanf("%d",&a[i]);

	printf("\n");
    heapsort(a,n);
	printf("The elements of the array after max_heapify function: ");
	for(i=0;i<n;i++)
		printf("%d ",a[i]);

	return 0;
}

void heapsort(int *a, int n){
    int i, temp = 0;
    build_max_heap(a,n);
    for(i=n-1;i>=1;i--){
        temp = a[i];
        a[i] = a[0];
        a[0] = temp;
        n = n-1;
        max_heapify(a,0,n);
    }

}

void build_max_heap(int *a, int n){
    int i;
    for(i=n/2;i>=0;i--)
		max_heapify(a,i,n);
}

void max_heapify(int *a, int i, int n){
	int l,r,largest=i,temp=0;
	l = 2*i+1;
	r = 2*i+2;
	if(l<n && a[l]>a[i])
		largest = l;
	if(r<n && a[r]>a[largest])
		largest = r;
	if(largest != i){
		temp = a[largest];
		a[largest] = a[i];
		a[i]= temp;
		max_heapify(a,largest,n);
	}
}

