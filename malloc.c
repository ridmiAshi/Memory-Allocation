#include <stdio.h>
#include "malloc.h"

#define MAX 25000

char memory_array[MAX];

// create linked list in same array
struct Node{
	int size;
	char free;
	struct Node *next;
	struct Node *prev;
	char *start_free;
};

typedef struct Node block_set;
block_set *root ;

// create first_block_set as first node of linked list
void first_block_set(){
	root = (block_set *)memory_array;
	root->free = 1;
	root->size = ( MAX - sizeof(block_set) );
	root->next = NULL;
	root->prev = NULL;
	root->start_free = memory_array + sizeof(block_set);
}

// function for split free memory cell
void separate_memory_block(block_set *current,unsigned int size){
	
	if(current->size - size <= sizeof(block_set))
		return;
	
	block_set *new =  ( (void *)(current + 1) + size);
	
	// set the links
	new->size =  current->size - size - sizeof(block_set); 
	new->free = 1;
	new->start_free = (void *)(new + 1);
	new->next = current->next;
	new->prev = current;
	
	if(current->next)
		current->next->prev = new;
	
	current->size = size;		
	current->next = new;
}


// function for merge free space
void combine_memory(block_set *temp){
	temp->size = (temp->size) + (temp->next->size) + (sizeof(block_set)) ;
	temp->next = temp->next->next;
	
	if(temp->next != NULL)
		temp->next->prev = temp;
}


// MALLOC function
void * MyMalloc(unsigned int size){

	if(size >= 0 && size <= MAX - sizeof(block_set)){

		//create first block_set
		if(root == NULL) 
			first_block_set();
		
		block_set *ptr = root;
		
		while(ptr != NULL){
			if(ptr->free == 1 && ptr->size >= size){
				if(ptr->size > size) 
					separate_memory_block(ptr,size);
				
				ptr->free = 0;
				return ptr->start_free;					
			}
			ptr = ptr->next;
		}
		return NULL;
	}
	else
		return NULL;
}


// FREE function 
void MyFree(void *target){
	
	block_set *ptr = root;
	
	while(ptr != NULL){
		if(ptr->start_free == target){
			ptr->free = 1;
			if((ptr->next != NULL) && (ptr->next->free == 1) )
				combine_memory(ptr);
			if((ptr->prev != NULL) && (ptr->prev->free == 1) )
				combine_memory(ptr->prev);
				return;
		}
		ptr = ptr->next;
	}
}


int main(){
	int *a =(int *) MyMalloc(100);
	printf("%d",a);
	MyFree(a);
	
	void *MyMalloc(unsigned int );
	void MyFree(void *);		
	
	return 0;
}
