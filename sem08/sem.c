#include <stdlib.h>
#include <stdio.h>

int main(void) {
  void *s = malloc(32 << 20);

  if (!s) {
    perror("malloc");
    exit(1);
  }
  
  free(s);
  return 0; 
}
