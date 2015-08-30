#include <stdlib.h>
void bench_D (int *v, int N, int M) {
  if (N < M) {
    int i;
    for (i = 0; i < N; i++) {
      v[i] = 0;
    }
    v[M] = 1;
  }
}



