#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int prev = STDIN_FILENO, d[2];
  for (int i = 1; i < argc; ++i) {
    if (pipe(d) == -1) {
      perror("");
      exit(1);
    }

    if (!fork()) {
      if (close(d[0]) == -1) {
        perror("");
        exit(1);
      }

      if (dup2(prev, STDIN_FILENO) == -1) {
        perror("");
        exit(1);
      }

      if (i != argc - 1) {
        if (dup2(d[1], STDOUT_FILENO) == -1) {
          perror("");
          exit(1);
        }
      }

      if (execlp(argv[i], argv[i], NULL) == -1) {
        perror("");
        exit(1);
      }
    } else {
      if (close(prev) == -1) {
        perror("");
        exit(1);
      }

      prev = d[0];
      
      if (close(d[1]) == -1) {
        perror("");
        exit(1);
      }
    }
  }

  for (int i = 1; i < argc; ++i) {
    wait(NULL);
  }

  return 0;
}
