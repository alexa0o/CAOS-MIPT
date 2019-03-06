#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>

char *strcat(char *dest, const char *src);

int main(int argc, char *argv[]) {
  DIR *dir;
  const char *dirname;
  if (argc == 1) {
    dirname = ".";
  } else {
    dirname = argv[1];
  }
  dir = opendir(dirname);
  if (dir != NULL) {
    struct dirent *dp;
    while ((dp = readdir(dir)) != NULL) {
      if (dp->d_type == DT_DIR) {
        printf("%s\n", dp->d_name);
      }
    }
  } else {
    perror(dirname);
  }
  return 0;
}
