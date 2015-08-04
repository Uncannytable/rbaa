#include <stdlib.h>
#include <stdio.h>
void foo(int* v){
  int a = 0;
  int b = 1;
  v[a] = 10;
  v[b] = 20;
  prinf("%d", v[a]);
}
void bar(int* v, int a, int b){
  v[a] = 10;
  v[b] = 20;
  prinf("%d", v[a]); 
}
int main(int argc, char** argv){
  int v = malloc(sizeof(int)*2);
  foo(v);
  bar(v, 0, 1);
}
