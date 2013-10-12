// Hash_table_ex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MEM_ERROR  1
#define STR_EXISTS 2
#define SUCCESS 0

struct hash_list{
	char* string;
	struct hash_list* list_next;
};

struct hash_table_t{
	int size;
	hash_list **table;
};

hash_table_t* hash_table_create(int table_size){
	hash_table_t* new_table;
	int i=0;
	if(table_size<1)
		return NULL;
/* Attempt to allocate memory for the table structure */
	if((new_table = (struct hash_table_t*)malloc(sizeof(struct hash_table_t ))) == NULL)
		return NULL;

/* Attempt to allocate memory for the table itself */
		if ((new_table->table = ((hash_list**)malloc(sizeof(hash_list *) * table_size))) == NULL)
			return NULL;

/* Initialize the elements of the table */
		for(i=table_size; i>0; i--) new_table->table[i] = NULL;
		new_table->size = table_size;
		return new_table;
}

/* The Hash function */
unsigned int hash(hash_table_t* hashtable, char* str){
	unsigned int hashval;
/* we start our hash out at 0 */
	hashval = 0;
/* for each character, we multiply the old hash by 31 and add the current character.  Remember that shifting a number left is equivalent to 
 * multiplying it by 2 raised to the number of places shifted.  So we are in effect multiplying hashval by 32 and then subtracting hashval.  
 * Why do we do this?  Because shifting and subtraction are much more efficient operations than multiplication.*/
	for(;*str!='\0';str++) hashval = *str + ((hashval << 5) - hashval);
	return (hashval % hashtable->size);
}

hash_list* lookup_string(hash_table_t *hashtable, char* str){
	hash_list* list = NULL;
	unsigned int hashval = hash(hashtable,str);
/* Go to the correct list based on the hash value and see if str is in the list.  If it is, return return a pointer to the list element.
 * If it isn't, the item isn't in the table, so return NULL. */
	for(list = hashtable->table[hashval]; list!= NULL; list=list->list_next){
		if(strcmp(str, list->string) == 0) return list;
	}
	return NULL;
}

int add_string(hash_table_t* hashtable, char* str){
	hash_list *new_list, *current_list;
	unsigned int hashval = hash(hashtable,str);
	current_list = lookup_string(hashtable,str);
	if(current_list != NULL) return STR_EXISTS;

	new_list = (hash_list*)malloc(sizeof(hash_list));
	if(new_list == NULL) return MEM_ERROR;

	new_list->string = _strdup(str);
	new_list->list_next = hashtable->table[hashval];
	hashtable->table[hashval] = new_list;
	return SUCCESS;
}

void free_table(hash_table_t* hashtable){
	hash_list * list, *temp;
	int i;
	if(hashtable == NULL) return;

	for(i=0;i < hashtable->size; i++){
		list = hashtable->table[i];
		while(list != NULL){
			temp = list;
			list = list->list_next;
			free(temp->string);
			free(temp);
		}
	}
	free(hashtable->table);
	free(hashtable);
	return;
}

int _tmain(int argc, _TCHAR* argv[])
{
	hash_table_t* hash_table;
	hash_list* list;
	int size_table = 12, result = 0;
	hash_table = hash_table_create(size_table);
	if(hash_table == NULL){ 
		printf("Table creation failed\n");
		return 0;
	}
	result = add_string(hash_table,"George");
	printf("The result of string addition is %d\n", result);
	result = add_string(hash_table,"Prashaant");
	printf("The result of string addition is %d\n", result);
	result = add_string(hash_table,"Ajay");
	printf("The result of string addition is %d\n", result);
	result = add_string(hash_table,"Karthik");
	printf("The result of string addition is %d\n", result);
	list = lookup_string(hash_table,"George");
	if(list == NULL) printf("String not present in the table\n");
	else printf("String present in the table\n");
	list = lookup_string(hash_table, "Shahina");
	if(list == NULL) printf("String not present in the table\n");
	else printf("String present in the table\n");
	getchar();
	return 0;
}

