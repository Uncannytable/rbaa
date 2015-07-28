#include <stdlib.h>
typedef struct{
  int x;
  int y;
} point;
void bench_G (point* s, point d) {
  s->x = d.x;
  s->y = d.y;
}
int main(int argc, char** argv)
{
  point* w = malloc(sizeof(point));
  point z;
  benc_G(w, z);
}



