#include <stdlib.h>
void bench_L(int *v) {
  int* i = v;
  int* e0 = v + 4;
  int* e1 = v + 40;
  while (i < e0) {
    *i = 0; i++;
  }
  while (i < e1) {
    *i = 1; i++;
  }
}
int main(int argc, char** argv)
{
  int x = malloc(100*sizeof(int));
  benc_L(x);
}

