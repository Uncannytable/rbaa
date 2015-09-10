#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(){
	int* p = malloc (7*sizeof(int));
	int *p1, *p2, a;
	*p = 8;
	a = 10;
	p1 = p + 4;
	p2 = p + 6;
	while(p2>p1){
		a = *p; //this instruction is sunk by instcombine whenever it is put after the store in p2
		*p2 = 4;
		//a = *p;
		p2--;
	}
	printf("a = %d, *p = %d \n",a, *p);
	return 0;
} 


