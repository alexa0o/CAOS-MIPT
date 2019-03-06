#include <stdio.h>
#include <stdlib.h>

void strings(FILE *f) {
  int c;
  int buff[3], current = 0;
  while ((c = fgetc(f)) != EOF) {
    if ((c < 32 && c != '\t') || c > 126) {
      if (c == '\n' && current > 2) {
        if (current == 3) {
          int i; 
          for (i = 0; i < 3; ++i) {
            putchar(buff[i]);
          }
        }
        putchar(c);
      }
      current = 0;
    } else {
      if (current < 3) {
        buff[current] = c;
        current = (current + 1) % 4;
      } else if (current == 3) {
        int i;
        for (i = 0; i < 3; ++i) {
          putchar(buff[i]);
        }
        putchar(c);
        current += 1;
      } else {
        putchar(c);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    strings(stdin);
  } else {
    int i;
    for (i = 1; i < argc; ++i) {
      FILE *f = fopen(argv[i], "r");
      if (f != NULL) {
        strings(f);
        fclose(f);
      } else {
        perror(argv[i]);
      }
    }
  }
  return 0;
}
