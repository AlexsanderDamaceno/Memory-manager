#include  "memory_mg.h"




Block *freelist = (Block *)NULL; 


size_t used_size = 0;

void initialize(){
  
        freelist         = (Block *)sbrk(0); 
        sbrk(HEADER_SZ + HEAP_SIZE);
        freelist->size   =  HEAP_SIZE; 
        freelist->isfree =  True; 
        freelist->next   =  NULL; 
        return; 
}





void split_block(Block *old_block , size_t size){
    
    Block *new_block =  (void*)old_block + sizeof(Block) + size;
    
    new_block->isfree = True; 
    new_block->size = old_block->size - sizeof(Block) - size; 

    old_block->isfree = False; 
    old_block->size = size; 
    old_block->next = new_block;
    return; 


}



Block * find_FreeBlock(size_t new_size){
    Block *current = freelist;  
   
    while(current != NULL && (current->size  < new_size || current->isfree == False)){
          current  = current->next;   
    }     

    return current; 

}


void *Alloc_block(size_t  size){
    
      if(!freelist){
         
          initialize(); 

      }

      
      
      Block * F_block = find_FreeBlock(size);

     
      if(F_block == NULL){
      	 printf("Error full heap space\n"); 
      	 return (void *)NULL; 
      }      


      if(F_block->size == size){
       F_block->isfree = False;
       used_size += sizeof(Block) + size; 
       
     	 return  User_addr(F_block); 
      
      }

     // case size < new_size 
     
      split_block(F_block , size);
      used_size += sizeof(Block) + size; 
      return User_addr(F_block);   
      


}




void merge(){
     

  struct Block *current_block = freelist; 

	while(current_block != NULL){
		
        if(!Is_free(current_block))
       	  {
                  Next_Block(current_block);
                  continue; 
	        }

	     if(Is_free(current_block) && current_block->next != NULL &&  Is_free(current_block->next))
	       {
		        current_block->size += current_block->next->size + sizeof(Block); 
		        current_block->next =  current_block->next->next; 
	         	current_block       =  current_block->next->next;
		        continue;
         }


        Next_Block(current_block);

		 

	}

}


void Free_block(void *ptr){
   
   if(ptr == NULL)
   {
   printf("Null pointer passed\n");
   return; 
   }



   if(ptr   <  sbrk(0)){
   	   ptr -= sizeof(Block);
	     Block *free_block = (Block *)ptr; 
   	   free_block->isfree = True; 
   	   used_size -= sizeof(Block) + free_block->size; 
   	   merge();

   }else{ 
      printf("Invalid pointer  on free\n"); 
   }

}


void heap_usage(){
   printf("Used bytes in heap: %ld\n" , used_size); 
   return; 
}


void show_Blocks(){
      
   Block *current_block =  freelist; 

   while(current_block != NULL){
           
      printf("chunk size: %ld\n", current_block->size);
      Next_Block(current_block);

   }
  
}


void * realloc_block(void *ptr , size_t size){
   
  
   
   Block *header = GET_HEADER(ptr);


  
   if(header->size >= size)
         return User_addr(ptr); 

   if(ptr != NULL && header->size < size){
       
       void *new_ptr  = (void *)Alloc_block(size); 
      
       if(!new_ptr){
         printf("Error at realloc\n");
         return ptr; 
       } 

       
       memcpy(new_ptr , ptr , header->size); 
       Free_block(ptr);
       return new_ptr; 
   }

}

void  *calloc_block(size_t num  , size_t type_size){
   

    if(num == 0 || type_size == 0)
        return NULL; 

    int newsize = type_size*num;
 

    void *block = Alloc_block(newsize); 
    memset(block , 0 , newsize);
    return block;



} 









