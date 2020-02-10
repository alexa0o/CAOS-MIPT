#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char ** argv){
	int fd = shm_open("mem", O_RDWR, 0600);
	if (fd == -1){
		perror("shm_open");
		exit(EXIT_FAILURE);
	} 
  char buf[1024];
  read(fd, buf, 1024);
  close(fd);
  puts(buf);
}
