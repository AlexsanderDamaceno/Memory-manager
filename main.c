

#include "memory_mg.h"


typedef struct a{
	int a; 
	int b; 
}test;

int main(){
  

  



  
  printf("dsfafsd%d\n" , sizeof(void *));
  int *a = Alloc_block(10*sizeof(int));
  show_Blocks();
  a[9] =10;
  printf("%d\n" , a[9]); 
  Free_block(a);
 
  
  

 
  
   


 
  
  return 1; 

}