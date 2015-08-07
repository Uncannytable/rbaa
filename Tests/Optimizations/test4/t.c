#include <stdlib.h>
#include <stdio.h>
void foo(){
  int* v = (int*)malloc(sizeof(int)*3);
  int* a = v + 1;
  int* b = v + 2;
  *a = 10;
  *b = 20;
  printf("%d", *a);
}
void bar(int* a, int* b){
  *a = 10;
  *b = 20;
  printf("%d", *a); 
}
int main(int argc, char** argv){
  foo();
  int* v = (int*)malloc(sizeof(int)*3);
  int* a = v + 1;
  int* b = v + 2;
  bar(a, b);
}
