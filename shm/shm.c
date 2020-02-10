#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  char cmnd;
  sscanf(argv[1], "%c", &cmnd);

  if (cmnd == 'c') {
    int fd = shm_open(argv[2], O_RDWR | O_CREAT, 0777);
    if (fd == -1) {
      perror("shm_open");
      exit(1);
    }

    ftruncate(fd, 64);
    close(fd);
  }

  if (cmnd == 'w') {
    int fd = shm_open(argv[2], O_RDWR, 0777);
    if (fd == -1) {
      perror("shm_open");
      exit(1);
    }

    char *mem = mmap(NULL, 64, PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
      perror("mmap");
      close(fd);
      exit(1);
    }

    strcpy(mem, argv[3]);

    if (munmap(mem, 64) == -1) {
      perror("munmap");
      close(fd);
      exit(1);
    }

    close(fd);
  }

  if (cmnd == 'r') {
    int fd = shm_open(argv[2], O_RDWR, 0777);
    if (fd == -1) {
      perror("shm_open");
      exit(1);
    }

    char *mem = mmap(NULL, 64, PROT_READ, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
      perror("mmap");
      close(fd);
      exit(1);
    }

    printf("%s:\n%s\n", argv[2], mem);
    
    if (munmap(mem, 64) == -1) {
      perror("munmap");
      close(fd);
      exit(1);
    }

    close(fd);
  }
  return 0;
}
