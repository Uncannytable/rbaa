struct array { int s, acc, *v; };
#include <stdlib.h>
void bench_C(struct array src) {
  int i;
  for (i = 0; i < src.s; i++) {
    src.acc += src.v[i];
  }
}



