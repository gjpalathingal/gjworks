/*
 * top.c
 *
 *  Created on: Sep 26, 2013
 *      Author: George
 */
/*Print the numbers between 30 to 3000.
CONSTRAINT:
The numbers should not contain digits either in increasing order or decreasing order.

FOLLOWING NOT ALLOWED
##123,234,345,1234,2345##increasing order,
##32,21,321,432,3210 etc##decreasing order.

FOLLOWING ALLOWED:
243,27,578,2344 etc.,
*/

#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>

void main(){
	int temp,i,j;
	FILE *f1 = fopen("output.txt","w");
	for(j=31;j<100;j++){
	temp = j%10;
	i=j/10;
	if(i%10 == temp+1 || i%10 == temp-1)
		continue;
	fprintf(f1,"%d\n",j);
	}
	for(j=100; j<1000; j++){
			temp = j%10;
			i=j/10;
			if(i%10 == temp+1){
					temp=i%10;
					i = i/10;
					if(i%10 == temp+1)
						continue;
			}
			if(i%10 == temp-1){
					temp = i%10;
					i = i/10;
					if(i%10 == temp-1)
						continue;
			}
			fprintf(f1,"%d\n",j);
		}
	for(j=1000; j<3000; j++){
		temp = j%10;
		i = i/10;
		if(i%10 == temp+1){
				temp=i%10;
				i = i/10;
				if(i%10 == temp+1){
					temp=i%10;
					i = i/10;
					if(i%10 == temp+1)
					continue;
				}
		}
		else if(i%10 == temp-1){
				temp = i%10;
				i = i/10;
				if(i%10 == temp-1){
					temp = i%10;
					i = i/10;
						if(i%10 == temp-1)
							continue;
					}
				}
		fprintf(f1,"%d\n",j);
		}
	fclose(f1);
	}
