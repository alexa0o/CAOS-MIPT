#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define true 1
#define false 0
#define min(x, y) (x) < (y) ? (x) : (y)

typedef unsigned long long ull;
typedef char bool;

bool *SetArrayOfPrime(ull limit) {
  int fd = shm_open("mem", O_RDWR | O_CREAT, 0777);
  if (fd == -1) {
    perror("shm_open");
    exit(1);
  }

  ftruncate(fd, limit);

  bool *mem = mmap(NULL, limit, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
  if (mem == MAP_FAILED) {
    perror("mmap");
    close(fd);
    exit(1);
  }

  for (ull i = 0; i < limit; ++i) {
    mem[i] = true;
  }

  return mem;
}

int main(int argc, char *argv[]) {
  ull limit = ULLONG_MAX, count_threads = 0;

  if (argc < 2) {
    perror("input error");
    exit(1);
  }
  if (argc == 3) {
    sscanf(argv[2], "%llu", &limit);
  }
  sscanf(argv[1], "%llu", &count_threads);

  ull array_limit = limit / 2 + limit % 2;
  ull chunk_size = 100;

  bool *is_prime = SetArrayOfPrime(array_limit);
  
  //for (ull i = 0; i < min(chunk_size, array_limit); ++i) {
  //  if (is_prime[i] == true) {
  //    for (ull j = 3; (2 * i + 3) * j - 3 < min(chunk_size, array_limit) * 2; j += 2) {
  //      is_prime[((2 * i + 3) * j - 3) / 2] = false;
  //    }
  //  }
  //}


  return 0;
}
