#include <stdlib.h>
int bench_B(int* v, int N) {
  int sOdd = 0;
  int sEven = 0;
  int i;
  for (i = 0; i < N; i += 2) {
    int* w = v + i;
    sOdd += *w;
    sEven += *(w + 1);
  }
  return sOdd > sEven ? 1 : 0;
}
int main(int argc, char** argv)
{
  int x = malloc(100*sizeof(int));
  bench_B(x, argc);
}



