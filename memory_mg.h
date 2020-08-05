#ifndef MEMORY_MG
#define MEMORY_MG 


#include <stddef.h> 
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define  HEAP_SIZE 0x100000

typedef struct Block{
 int isfree; 
 size_t size; 
 struct Block *next; 
 
 }Block;


#define WORD_SIZE void *

#define HEADER_SZ sizeof(Block)
#define GET_HEADER(Block) ((void*)Block - HEADER_SZ) 

#define DEBUG_STR(value)  printf("%s\n" , value)
#define DEBUG_ADDR(value) printf("0x%x\n" , value)


#define True  1
#define False 0

#define Is_free(Block) ((Block)->isfree == True)
#define Next_Block(Block)(Block = Block->next)


void initialize(); 

void  *Alloc_block(size_t size);
void  *realloc_block(void *ptr , size_t size);
void  *calloc_block(size_t num  , size_t type_size);
Block *verify_cache(size_t size);
void split_block(Block *old_block , size_t  size); 
void Free_block(void *ptr);
void merge(); 
void heap_usage();
void show_Blocks();
void put_Block_incache(Block *block);
size_t align(size_t bsize);

#define User_addr(block) (void *)( (void*)block + sizeof(Block)) 




#endif