#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int i;
  for (i = 1; i < argc; ++i) {
    if (fork() == 0) {
      execlp(argv[i], argv[i], NULL);
      exit(0);
    }
    wait(NULL);
  }
  return 0;
}
