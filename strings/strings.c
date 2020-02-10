#include <stdio.h>
#include <stdlib.h>

void strings(FILE *f) {
  int length = 1024;
  char buff[4], str[length], current = 0;
  buff[3] = '\0';
  str[length - 1] = '\0';
  while (fgets(str, length, f) != NULL) {
    int i;
    for (i = 0; str[i] != '\0'; ++i) {
      char c = str[i];
      if ((c < 32 && c != '\t') || c > 126) {
        if (current > 3) {
          putchar('\n');
        }
        current = 0;
      } else {
        if (current < 3) {
          buff[current] = c;
          current = (current + 1) % 4;
        } else if (current == 3) {
          fputs(buff, stdout);
          putchar(c);
          current += 1;
        } else {
          putchar(c);
        }
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
