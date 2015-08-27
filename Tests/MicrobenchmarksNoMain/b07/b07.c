#include <stdlib.h>
typedef struct{
  int x;
  int y;
} point;
void bench_G (point* s, point d) {
  s->x = d.x;
  s->y = d.y;
}



