#include <stdlib.h>
void bench_J(char* p) {
  int i;
  for (i = 0; i < 4; i++) {
    p[i] = -1;
  }
  p[i] = 0;
  i++;
  for (; i < 10; i++) {
    p[i] = 1;
  }
} 



