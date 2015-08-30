#include <stdlib.h>
void bench_F(int* v, int N) {
  int i = 0, j = N - 1;
  while (i < N) {
    if (v[i] > v[j]) {
      int tmp = v[i];
      v[i] = v[j];
      v[j] = tmp;
    }
  }
} 



