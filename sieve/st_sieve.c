#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <limits.h>
#include <time.h>

typedef unsigned long long ull;

struct Info {
  ull from;
  ull to;
  ull d;
};

char *digits;

void *check(void *inf) {
  struct Info *info = inf;
  for (ull i = info->from; i < info->to; i += info->d) {
    digits[i] = 0;
  }
  pthread_exit(NULL);
}

void sieve(ull thrds_cnt, ull max_value) {
  digits = malloc((max_value + 1));
  if (digits == NULL) {
    perror("");
    exit(1);
  }
  for (ull i = 2; i <= max_value; ++i) {
    digits[i] = 1;
  }
  digits[0] = 0;
  digits[1] = 0;
  struct Info info[thrds_cnt];
  pthread_t threads[thrds_cnt];
  for (ull i = 2; i <= max_value; ++i) {
    if (digits[i] == 1) {
      printf("%llu\n", i);
      ull count = (max_value) / i;
      ull chunk = (count + thrds_cnt - 1) / thrds_cnt;
      int j;
      for (j = 0; j < thrds_cnt; ++j) {
        info[j].from = i + j * chunk * i;
        info[j].to = i + (j + 1) * chunk * i;
        info[j].d = i;
        info[j].to = info[j].to > max_value + 1 ? max_value + 1 : info[j].to;
        if (pthread_create(&threads[j], NULL, check, &info[j]) != 0) {
          perror("");
          exit(1);
        }

        if (info[j].to > max_value) {
          break;
        }
      }
      while (j >= 0) { 
        if (pthread_join(threads[j], NULL) != 0) {
          perror("");
          exit(1);
        }
        --j;
      }
    }
  }
  free(digits);
}

int main(int argc, char *argv[]) {
  ull limit = ULONG_MAX;
  int cnt_thrds;
  if (argc == 3) {
    sscanf(argv[2], "%llu", &limit);
  }
  sscanf(argv[1], "%d", &cnt_thrds);
  sieve(cnt_thrds, limit);
}
