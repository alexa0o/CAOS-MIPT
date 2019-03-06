#include <stdio.h>

void cat(FILE *file) {
  int c;
  while ((c = fgetc(file)) != EOF) {
    putc(c, stdout);
  }
}

int main(int argc, char *argv[]) {
  FILE *file;
  if (argc == 1) {
    file = stdin;
    cat(file);
  } else {
    int i;      
    for (i = 1; i < argc; ++i) {
      file = fopen(argv[i], "r");
      if (file != NULL) {
        cat(file);
        fclose(file);
      } else {
        perror(argv[i]);
      }
    }
  } 
  return 0;
}
