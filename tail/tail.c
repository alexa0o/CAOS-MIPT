#include <stdio.h>
#include <stdlib.h>

void Tail(FILE *f) {
  unsigned current = 0;
  const unsigned max_length = 1000000;
  char *tail[10];
  int flag = 0;

  int i;
  for (i = 0; i < 10; ++i) {
    tail[i] = (char*) malloc(max_length);
  }

  while (fgets(tail[current], max_length, f) != NULL) {
    current = (current + 1) % 10;
    if (current == 9) {
      flag = 1;
    }
  }

  if (flag == 1) {
    for (i = current; i < 10; ++i) {
      printf("%s", tail[i]);
    }
  }
  for (i = 0; i < current; ++i) {
    printf("%s", tail[i]);
  }

  for (i = 0; i < 10; ++i) {
    free(tail[i]);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    Tail(stdin);
  } else {
    int i;
    for (i = 1; i < argc; ++i) {
      printf("==> %s <==\n", argv[i]);
      FILE *f = fopen(argv[i], "r");
      if (f == NULL) {
        perror(argv[i]);
      } else {
        Tail(f);
        fclose(f);
      }
    }
  }
  return 0;
}
