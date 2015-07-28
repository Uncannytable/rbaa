#include <stdlib.h>
void bench_I(int **v, int N) {
  int i;
  for (i = 0; i < N; i += 2) {
    int* t0 = v[i];
    int* t1 = v[i + 1];
    int j;
    for (j = 0; j < N; j++) {
      t0[j] = t1[j];
    }
  }
} 
int main(int argc, char** argv)
{
  int x = malloc(100*sizeof(int));
  bench_I(x, argc);
}



