#include "memory_mg.h"


typedef struct a{
	int a; 
	int b; 
}test;

int main(){
  

  



  
 
  test *a = Alloc_block(sizeof(test));
  
  
                

  a->a = 1000; 
  a->b = 10;

  
  a = (test *)realloc_block(a , sizeof(test)+100);

  printf("%d\n" , (a)->a);
  
   


 
  
  return 1; 

}