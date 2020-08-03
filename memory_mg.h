#ifndef MEMORY_MG
#define MEMORY_MG 


#include <stddef.h> 
#include <stdio.h>


typedef struct Block{
 int isfree; 
 size_t size; 
 struct Block *next; 
 }Block;


#define True  1
#define False 0

#define Is_free(Block) ((Block)->isfree == True)
#define Next_Block(Block)(Block = Block->next)


void initialize(); 

void * Alloc_block(size_t size);
void   split_block(Block *old_block , size_t  size); 
void   Free_block(void *ptr);
void   merge(); 
void   heap_usage();
void   show_Blocks();



#define User_addr(block) (void *)( (void*)block + sizeof(Block)) 




#endif