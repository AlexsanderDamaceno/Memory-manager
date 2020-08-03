#include  "memory_mg.h"

char memory[10000]; 


Block *freelist = (Block *)memory; 


size_t used_size = 0;

void initialize(){
	freelist->size = sizeof(memory) - sizeof(Block); 
	freelist->isfree = True; 
	freelist->next = NULL; 
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
    
      if(freelist->size == 0){
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
      return  User_addr(F_block);   
      


}




void merge(){
     

  struct Block *current_block = freelist; 

	while(current_block != NULL){
		 if(!Is_free(current_block))
		 {
            Next_Block(current_block);
            continue; 
		 }

		 if(Is_free(current_block) && current_block->next != NULL &&  Is_free(current_block->next)){
		    
		    current_block->size += current_block->next->size + sizeof(Block); 
        current_block->next =  current_block->next->next; 
        current_block       =  current_block->next->next;
        continue;

		 }

     Next_Block(current_block);

		 

	}

}


void Free_block(void *ptr){
   
   
   if(ptr <= (void*)(memory + sizeof(memory))){
   	   ptr -= sizeof(Block);
       Block *free_block = (Block *)ptr; 
   	   free_block->isfree = True; 
   	   used_size -= sizeof(Block) + free_block->size; 
       merge();

   }else{ 
      printf("Invalid pointer  on free"); 
   }


}


void heap_usage(){
   printf("Used bytes in heap: %ld" , used_size); 
   return; 
}


void show_Blocks(){
      
   Block *current_block =  freelist; 

   while(current_block != NULL){
           
      printf("chunk size: %ld\n", current_block->size);
      Next_Block(current_block);

   }
  
}