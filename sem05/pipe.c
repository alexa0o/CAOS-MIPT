#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void son(const char *s, int from, int to, int cl) {
  if (!fork()) {
    dup2(from, 0);
    dup2(to, 1);
    close(cl);
    execlp(s, s, NULL);
  }
}

int main(int argc, char *argv[]) {
  int d[2];
  pipe(d);
  son(argv[1], 0, d[1], d[0]);
  son(argv[2], d[0], 1, d[1]);
  close(d[0]);
  close(d[1]);
  wait(NULL);
  wait(NULL);
  printf("Done\n");
  return 0;
}
