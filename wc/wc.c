#include <stdio.h>

int isspace(int c);

void PrintFileInfo(FILE *f, const char *fname) {
  unsigned long count_words = 0, count_lines = 0, count_symbols = 0;
  int c;
  char isword = 0;
  while ((c = getc(f)) != EOF) {
    ++count_symbols;
    if (isspace(c) != 0) {
      if (c == '\n') {
        ++count_lines;
      }
      isword = 0;
    } else if (!isword) {
      isword = 1;
      ++count_words;
    }
  }
  printf("%lu %lu %lu %s\n", count_lines, count_words, count_symbols, fname);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    PrintFileInfo(stdin, "");
  } else {
    int i;
    for (i = 1; i < argc; ++i) {
      FILE *f = fopen(argv[i], "r");
      if (f != NULL) {
        PrintFileInfo(f, argv[i]);
        fclose(f);
      } else {
        perror(argv[i]);
      }
    }
  }
  return 0;
}
