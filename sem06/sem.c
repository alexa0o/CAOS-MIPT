#include <pthread.h>
#include <stdio.h>

int a = 0;
pthread_mutex_t m;

void *f(void *p) {
  int i, s = 0;
  for (i = 0; i < 10000000; ++i) {
    ++s;
  }
  pthread_mutex_lock(&m);
  a += s;
  pthread_mutex_unlock(&m);
  return p;
}

int main(void) {
  pthread_t t1, t2;
  pthread_mutex_init(&m, NULL);
  pthread_create(&t1, NULL, f, NULL);
  pthread_create(&t2, NULL, f, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_mutex_destroy(&m);
  printf("a=%d\n", a);
  return 0;
}
