#include <stdio.h>
#include <stdlib.h>
int no_body_sum(int* x, int* y);
int main(int argc, char** argv) {
  int* v = (int*)malloc(6 * sizeof(int));
  int a = 0, b = 1;
  v[0] = 13;
  v[1] = 17;
  v[a] += v[b];
  v[a] += v[b];
  no_body_sum(v, v + 1);
}
