#include <stdlib.h>
void bench_H (int *d, int *s, int N) {
  int i;
  for (i = 0; i < N; i++) {
    d[i] = s[i];
  }
}
int main(int argc, char** argv)
{
  int x = malloc(100*sizeof(int));
  int u = malloc(100*sizeof(int));
  bench_H(x, u, argc);
}



