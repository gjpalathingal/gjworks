/*
 * test.c
 *
 *  Created on: Sep 25, 2013
 *      Author: George
 */

#include<stdlib.h>
#include<stdio.h>
#include<stddef.h>

struct bintree{
	int data;
	struct bintree *righttree, *lefttree;
};

typedef struct bintree node;

void insert(node **tree, int val){
	node *temp;
	if(!(*tree)){
		temp = (node*)malloc(sizeof(node));
		temp->lefttree = NULL;
		temp->righttree = NULL;
		temp->data = val;
		*tree = temp;
		printf("Value %d added to the tree\n",val);
		return;
	}
	if(val < (*tree)->data)
		insert(&((*tree)->lefttree),val);
	else
		insert(&((*tree)->righttree),val);
	}


void print_preorder(node *tree){
	if(tree){
		printf("%d\n",tree->data);
		print_preorder(tree->lefttree);
		print_preorder(tree->righttree);
	}
}

void print_inorder(node *tree){
	if(tree){
		print_inorder(tree->lefttree);
		printf("%d\n",tree->data);
		print_inorder(tree->righttree);
	}
}

void print_postorder(node *tree){
	if(tree){
		print_inorder(tree->lefttree);
		print_inorder(tree->righttree);
		printf("%d\n",tree->data);
	}
}

void deletetree(node *tree){
	if(tree){
		deletetree(tree->lefttree);
		deletetree(tree->righttree);
		free(tree);
		printf("Node deleted\n");
	}
}

node* search(node *tree, int val){
	if(tree){
		printf("The current node data is %d\n",tree->data);
		if(val == tree->data)
				return tree;
		else if(val < tree->data)
				search(tree->lefttree, val);
		else
				search(tree->righttree, val);
	}
	else{
		printf("Node not found\n");
		return NULL;
	}
}

int main()
{
	node *temp = NULL;
	node *root = NULL;
	insert(&root, 9);
	insert(&root, 21);
	insert(&root, 90);
	insert(&root, 1);
	insert(&root, 67);
	insert(&root, 37);
	insert(&root, 92);
	insert(&root, -77);
	print_inorder(root);
	printf("The root of the tree is %d\n",root->data);
	temp = search(root,21);
	printf("The node found at %p\n", temp);
	deletetree(root);
	return 0;
}
