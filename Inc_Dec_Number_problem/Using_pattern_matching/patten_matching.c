/*
 * patten_matching.c
 *
 *  Created on: Sep 27, 2013
 *      Author: George
 */
/*
 * pattern_matching.c
 *
 *  Created on: Sep 27, 2013
 *      Author: George
 */
#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<string.h>

char *base_string1 ="0123456789";
char *base_string2 ="9876543210";

int match_string(int num){
	char *result = NULL;
	char *string_eq = (char*)malloc(sizeof(char)*10);
	string_eq = itoa(num,string_eq,10);
//	printf("%s\n",string_eq);
	result = strstr(base_string1,string_eq);
	if(result)
		return 1;
	else{
		result = strstr(base_string2, string_eq);
		if(result)
			return 1;
	}
	return 0;
}

int main()
{
	int flag = 0,i;
	FILE *f1 = fopen("pattern_output.txt","w");
	for(i=31;i<3000;i++){
		flag = match_string(i);
		if(flag == 1)
			continue;
		fprintf(f1,"%d\n",i);
	}
	fclose(f1);
}


