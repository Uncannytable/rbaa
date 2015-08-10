#include <stdlib.h>
int main(int argc, char** argv) {
  int i = 0;
  char* v = (char*)malloc(argc * 2);
  for (; i < argc; i++) {
    int j = 0;
    v[i] = 0;
    for (; j < argc; j++) {
      v[i] += v[j + argc];
    }
  }
}
