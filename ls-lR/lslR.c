#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_SIZE 256

#define S_IFSOCK 0140000   
#define S_IFLNK 0120000   
#define S_IFREG 0100000  
#define S_IFBLK 0060000 
#define S_IFDIR 0040000
#define S_IFCHR 0020000   
#define S_IFIFO 0010000 
#define S_IFMT 0170000

int dircomp(const void *p1, const void *p2) {
  return strcmp((*(struct dirent**) p1)->d_name, (*(struct dirent**) p2)->d_name);;
}

void print_info(const char *name, char *type, const char *path) {
    struct stat st;

    if (lstat(path, &st) == -1) {
      perror("");
      exit(1);
    }
    
    switch (st.st_mode & S_IFMT) {
      case S_IFSOCK:
        *type = 's';
        break;
      case S_IFLNK:
        *type = 'l';
        break;
      case S_IFREG:
        *type = '-';
        break;
      case S_IFBLK:
        *type = 'b';
        break;
      case S_IFDIR:
        *type = 'd';
        break;
      case S_IFCHR:
        *type = 'c';
        break;
      case S_IFIFO:
        *type = 'p';
        break;
    }

    char permissions[10];
    permissions[0] = st.st_mode & S_IRUSR ? 'r' : '-';
    permissions[1] = st.st_mode & S_IWUSR ? 'w' : '-';
    permissions[2] = st.st_mode & S_IXUSR ? 'x' : '-';
 
    permissions[3] = st.st_mode & S_IRGRP ? 'r' : '-';
    permissions[4] = st.st_mode & S_IWGRP ? 'w' : '-';
    permissions[5] = st.st_mode & S_IXGRP ? 'x' : '-';
 
    permissions[6] = st.st_mode & S_IROTH ? 'r' : '-';
    permissions[7] = st.st_mode & S_IWOTH ? 'w' : '-';
    permissions[8] = st.st_mode & S_IXOTH ? 'x' : '-';

    permissions[9] = '\0';

    struct passwd *pw = getpwuid(st.st_uid);
    if (pw == NULL) {
      perror("");
      exit(1);
    }

    struct group *gr = getgrgid(st.st_gid);
    if (gr == NULL) {
      perror("");
      exit(1);
    }

    printf("%c%s %ld ", *type, permissions, st.st_nlink);
    if (pw->pw_name == NULL) {
      printf("%u ", st.st_uid);
    } else {
      printf("%s ", pw->pw_name);
    }
    if (gr->gr_name == NULL) {
      printf("%u ", st.st_gid);
    } else {
      printf("%s ", gr->gr_name);
    }
    printf("%ld %s", st.st_size, name);
    if (*type == 'l') {
      char file_name[MAX_SIZE];
      if (readlink(path, file_name, MAX_SIZE) == -1) {
        perror("");
        exit(1);
      } else {
        printf(" -> %s", file_name); 
      }
    }
    printf("\n");
}

void ls(DIR *dir, const char *path, size_t len) {
  struct dirent **dp = (struct dirent **) malloc(MAX_SIZE * sizeof(struct dirent*));
  char *types = (char *) malloc(MAX_SIZE);
  char *next_path = (char *) malloc(len + MAX_SIZE + 2);
  size_t curr = 0, size = MAX_SIZE;

  while ((dp[curr++] = readdir(dir)) != NULL) {
    if (curr == size) {
      size *= 2;
      dp = (struct dirent **) realloc(dp, size * sizeof(struct dirent *));
      if (dp == NULL) {
        perror("");
        exit(1);
      }
      types = (char *) realloc(types, size);
      if (types == NULL) {
        perror("");
        exit(1);
      }
    }
    if (dp[curr - 1]->d_name[0] == '.') {
      curr--;
    }
  }

  if (curr > 1) {
    qsort(dp, curr - 1, sizeof(struct dirent*), dircomp);

    for (int i = 0; i < curr - 1; ++i) {
      sprintf(next_path, "%s/%s", path, dp[i]->d_name);
      print_info(dp[i]->d_name, &types[i], next_path);
    }

    for (int i = 0; i < curr - 1; ++i) {
      if (types[i] == 'd') {
        sprintf(next_path, "%s/%s", path, dp[i]->d_name);
        DIR *next_dir = opendir(next_path);
        if (next_dir == NULL) {
          perror("");
          exit(1);
        } 
        printf("\n%s:\n", next_path);
        ls(next_dir, next_path, len + 1 + strlen(dp[i]->d_name));
        closedir(next_dir);
      }
    }
  }
  
  free(next_path);
  free(types);
  free(dp);
}

int main(int argc, char *argv[]) {
  char *file;
  if (argc < 2) {
    file = ".";
  } else {
    file = argv[1];
  }
  DIR *dp = opendir(file);
  if (dp == NULL) {
    char tmp;
    print_info(file, &tmp, file);
  } else {
    printf("%s:\n", file);
    size_t len = strlen(file);
    if (file[len - 1] == '/') {
      file[len - 1] = '\0';
      len -= 1;
    }
    ls(dp, file, len);
    closedir(dp);
  }
  return 0;
}

#undef MAX_SIZE

#undef S_IFSOCK 
#undef S_IFLNK 
#undef S_IFREG 
#undef S_IFBLK 
#undef S_IFDIR 
#undef S_IFCHR 
#undef S_IFIFO 
#undef S_IFMT
