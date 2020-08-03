#include "memory_mg.h"


typedef struct a{
	int a; 
	int b; 
}test;

int main(){
  

  



  
 
  test *a = Alloc_block(1000);
  test *b = Alloc_block(1000);  
 
  Free_block(a);
  Free_block(b);

 
  show_Blocks();

  return 1; 

}