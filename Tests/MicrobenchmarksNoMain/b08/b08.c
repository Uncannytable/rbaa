#include <stdlib.h>
void bench_H (int *d, int *s, int N) {
  int i;
  for (i = 0; i < N; i++) {
    d[i] = s[i];
  }
}



