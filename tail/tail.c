#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Tail(FILE *f) {
  const unsigned length = 100000;
  unsigned current = 0, len = length;
  char *tail[10], buff[length + 1];
  int flag = 0;
  
  int i;
  for (i = 0; i < 10; ++i) {
    tail[i] = (char*) malloc(length);
    if (tail[i] == NULL) {
      perror("malloc");
      exit(1);
    }
  }

  while (fgets(buff, length + 1, f) != NULL) {
    if (buff[length - 1] == '\0') {
      if (len == length) {
        strcpy(tail[current], buff);
      } else {
        tail[current] = (char*) realloc(tail[current], len);
        if (tail[current] == NULL) {
          perror("realloc");
          exit(1);
        }
        strcat(tail[current], buff);
        len = length;
      }

      current = (current + 1) % 10;
      if (current == 9) {
        flag = 1;
      }
    } else {
      if (len == length) {
        strcpy(tail[current], buff);
      } else {
        tail[current] = (char*) realloc(tail[current], len);
        if (tail[current] == NULL) {
          perror("realloc");
          exit(1);
        }
        strcat(tail[current], buff);
      }
      len += length;
      buff[length - 1] = '\0';
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

  for (int i = 0; i < 10; ++i) {
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
