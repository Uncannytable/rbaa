#include <stdlib.h>
void bench_A(int *a) {
  for (int i = 0; i < 10; ++i)
    a[i + 1] = a[i + 2];
} 
int main(int argc, char** argv)
{
  int x = malloc(100*sizeof(int));
  bench_A(x);
}

