#include <stdlib.h>
int bench_E(int *v, int a) {
  int N = a % 2 ? 0 : 1;
  int M = a % 2 ? 2 : 3;
  int t0 = v[N];
  int t1 = v[M];
  return t0 + t1;
} 
int main(int argc, char** argv)
{
  int x = malloc(100*sizeof(int));
  bench_E(x, argc);
}



