#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int isspace(int c);

int qstrcmp(const void *s1, const void *s2) {
  return strcmp(*(char* const*) s1, *(char* const*) s2);
}

char **AddStr(char **strs, int *size, char **str, int *len) {
  strs = realloc(strs, (*size + 1) * sizeof(char*));
  if (strs == NULL) {
    perror("Bad alloc\n");
    exit(12);
  }
  *str = realloc(*str, *len + 1);
  if (*str == NULL) {
    perror("Bad alloc\n");
    exit(12);
  }
  (*str)[*len] = '\0';
  strs[(*size)++] = *str;
  *len = 0;
  *str = NULL;
  return strs;
}

char *AddChar(char *str, int *len, int c) {
  str = realloc(str, *len + 1);
  if (str == NULL) {
    perror("Bad alloc\n");
  }
  str[(*len)++] = c;
  return str;
}

int main(void) {
  char **strs = NULL, *str = NULL;
  int len = 0, size = 0;
  int c, isopen1 = 0, isopen2 = 0;

  while((c = getchar()) != EOF) {
    if (isopen1 == 0 && isopen2 == 0 && isspace(c) != 0) {
      // is space
      if (str != NULL) {
        strs = AddStr(strs, &size, &str, &len);
      }
    } else if (isopen1 == 0 && isopen2 == 0) {
      // no space
      if (c == '\'') {
        isopen1 = 1;
        if (len == 1 && (str[0] == ';' || str[0] == '&' || str[0] == '|')) {
          strs = AddStr(strs, &size, &str, &len);
        }
      } else if (c == '"') {
        isopen2 = 1;
        if (len == 1 && (str[0] == ';' || str[0] == '&' || str[0] == '|')) {
          strs = AddStr(strs, &size, &str, &len);
        }
      } else {
        if (str == NULL) {
          str = AddChar(str, &len, c);
        } else {
          if (c == ';') {
            strs = AddStr(strs, &size, &str, &len);
            str = AddChar(str, &len, c);
            strs = AddStr(strs, &size, &str, &len);
          } else if (c == '&' || c == '|') {
            if (len == 1 && str[0] == c) {
              str = AddChar(str, &len, c);
              strs = AddStr(strs, &size, &str, &len);
            } else {
              strs = AddStr(strs, &size, &str, &len);
              str = AddChar(str, &len, c);
            }
          } else {
            if (len == 1 && (str[0] == '&' || str[0] == '|' || str[0] == ';')) {
              strs = AddStr(strs, &size, &str, &len);
            }
            str = AddChar(str, &len, c);
          }
        }
      }
    } else {
      if (isopen1 == 1 && c == '\'') {
        isopen1 = 0;
      } else if (isopen2 == 1 && c == '"') {
        isopen2 = 0;
      } else {
        str = AddChar(str, &len, c);
      }
    }
  }

  if (isopen1 == 1 || isopen2 == 1) {
    perror("Syntax error\n");
  }

  if (str != NULL) {
    strs = AddStr(strs, &size, &str, &len);
  }

  qsort(strs, size, sizeof(char*), qstrcmp);

  int i;
  for (i = 0; i < size; ++i) {
    printf("\"%s\"\n", strs[i]);
    free(strs[i]);
  }
  free(strs);
  return 0;
}
