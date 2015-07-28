#include <stdio.h>
#include <stdlib.h>
int no_body_sum(int* x, int* y);
int main(int argc, char** argv) {
  int* v = (int*)malloc(6 * sizeof(int));
  int a, b;
  v[0] = argc;
  v[1] = argc;
  switch(argc % 3) {
    case 0:  a = 0; b = 3; break;
    case 1:  a = 1; b = 4; break;
    default: a = 2; b = 5;
  }
  v[a] += v[b];
  v[a] += v[b];
  no_body_sum(v, v + 1);
}
