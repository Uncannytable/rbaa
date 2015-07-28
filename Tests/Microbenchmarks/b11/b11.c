#include <stdlib.h>
void bench_K(char *v) {
  char* m0 = malloc(10);
  char* m1 = malloc(10);
  int i;
  for (i = 0; i < 10; i++) {
    m0[i] = v[2*i];
    m1[i] = v[2*i + 1];
  }
}
int main(int argc, char** argv)
{
  int p = malloc(100);
  bench_K(p);
}



