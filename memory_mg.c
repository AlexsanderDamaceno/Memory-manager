#include  "memory_mg.h"


/*

Block *cacheList[] = {
    NULL ,
    NULL , 
    NULL , 
    NULL ,
    NULL , 
    NULL 
};
*/


Block *freelist = (Block *)NULL; 

size_t used_size = 0;


size_t align(size_t bsize){
   return  (bsize + sizeof(WORD_SIZE) - 1) & ~(sizeof(WORD_SIZE)-1);
}



void initialize(){
  
       
        freelist = (Block *)sbrk(0);

        sbrk(align(HEAP_SIZE));
        memset(freelist  , 0 , sizeof(align(HEAP_SIZE)));
        freelist->size   =  HEAP_SIZE; 
        freelist->isfree =  True; 
        freelist->next   =  NULL; 
        return; 
}

/*

void put_Block_incache(Block *block){


   Block *frlist = NULL; 
  
   if(block->size  == 8)
   {
     frlist  =    cacheList[0];
   }else if(block->size == 16){

      frlist =   cacheList[1]; 

   }else if(block->size == 32){
      frlist =  cacheList[2];

   }else if(block->size ==  64){
      frlist =  cacheList[3]; 

   }else if(block->size == 128){
      frlist =  cacheList[4];

   }else {
      frlist =  cacheList[5]; 
   }


   if(!frlist)
    {

     
      frlist = block;  
      block->next = NULL;
      return; 
    }


    Block *current = frlist; 
    
    while(current->next != NULL){
          current  = current->next;   
    }     

    current->next = block;
    block->next = NULL; 


    return;







}
*/


void split_block(Block *old_block , size_t size){
    
    Block *new_block =  (void*)old_block + sizeof(Block) + size;
    
    new_block->isfree = True; 
    new_block->size = old_block->size - sizeof(Block) - size; 
   
   
    old_block->isfree = False; 
    old_block->size = size; 
    old_block->next = new_block;

    return; 


}
/*

Block *verify_cache(size_t size){
  
   Block *frlist = NULL; 
  
   if(size  == 8)
   {
     frlist  =    cacheList[0];
   }else if(size == 16){
      printf("alexsander");
      frlist =   cacheList[1]; 
   }else if(size == 32){
      frlist =  cacheList[2];
   }else if(size ==  64){
      frlist =  cacheList[3]; 
   }else if(size == 128){
      frlist =  cacheList[4];
   }else {
      frlist =  cacheList[5]; 
   }

    

    Block *current =  frlist;
    while(current != NULL && (current->size  < size || current->isfree == False)){
          current  = current->next;   
    }  
   // printf("%x\n" , current);
    return current;


}

*/

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
      
      size = align(size);
      
      
      Block * F_block = NULL;

     // if(!(F_block = verify_cache(size)))
         F_block = find_FreeBlock(size);
      

   

     
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
  int c = 0; 

	
  while((current_block != NULL)) {
        
        

        c++;
       
             
		     
        if(!Is_free(current_block))
       	  {
                  Next_Block(current_block);
                  continue; 
	        }

        
      
	     if(Is_free(current_block) && current_block->next != NULL &&  Is_free(current_block->next))
	       {

		        current_block->size += current_block->next->size + sizeof(Block); 
		        
            Block *tmp = current_block->next->next;  
            current_block->next =  current_block->next->next; 

	          current_block       =  tmp;
            
            
		        continue;
         }else 

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
       
    //   put_Block_incache(free_block);

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









