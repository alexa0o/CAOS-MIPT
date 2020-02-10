#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

#define MAX_SIZE 256

typedef struct info {
  char atrr[11];
  long count_link, size;
  char owner[MAX_SIZE], group[MAX_SIZE];
  char name[MAX_SIZE], path[5 * MAX_SIZE];
} info;

int mycmp(const void *p1, const void *p2) {
  return strcmp((*(info *const *) p1)->name, (*(info *const *) p2)->name);
}

void get_info(info **f, const char *dir, const char *name) {
  struct stat st;
  struct group *gr;

  *f = (info*) malloc(sizeof(info));
  if (*f == NULL) {
    perror("malloc");
    exit(1);
  }

  strcpy((*f)->name, name);

  char type, owner_perm[4] = {0}, group_perm[4] = {0}, other_perm[4] = {0};

  sprintf((*f)->path, "%s/%s", dir, name);
  if (lstat((*f)->path, &st) != 0) {
    perror("");
    exit(1);
  }

  sprintf((*f)->owner, "%s", getpwuid(st.st_uid)->pw_name);

  if (S_ISLNK(st.st_mode)) {
    type = 'l';
    char tmp[MAX_SIZE] = {0};
    if (readlink((*f)->path, tmp, MAX_SIZE) == -1) {
      perror("");
      exit(1);
    }
    sprintf((*f)->name, "%s -> %s", (*f)->name, tmp);
  } else if (S_ISDIR(st.st_mode)) {
    type = 'd';
  } else {
    type = '-';
  } 

  owner_perm[0] = st.st_mode & S_IRUSR ? 'r' : '-';
  owner_perm[1] = st.st_mode & S_IWUSR ? 'w' : '-';
  owner_perm[2] = st.st_mode & S_IXUSR ? 'x' : '-';

  group_perm[0] = st.st_mode & S_IRGRP ? 'r' : '-';
  group_perm[1] = st.st_mode & S_IWGRP ? 'w' : '-';
  group_perm[2] = st.st_mode & S_IXGRP ? 'x' : '-';

  other_perm[0] = st.st_mode & S_IROTH ? 'r' : '-';
  other_perm[1] = st.st_mode & S_IWOTH ? 'w' : '-';
  other_perm[2] = st.st_mode & S_IXOTH ? 'x' : '-';
    
  sprintf((*f)->atrr, "%c%s%s%s", type, owner_perm, group_perm, other_perm);

  gr = getgrgid(st.st_gid);
  if (gr != NULL) {
    strcpy((*f)->group, gr->gr_name);
  } else {
    sprintf((*f)->group, "%d", st.st_gid);
  }

  (*f)->count_link = st.st_nlink;
  (*f)->size = st.st_size;
}


void ls(DIR *dir, const char *dirName) {
  struct dirent *dp;
  info *files[5 * MAX_SIZE];
  int curr = 0;  

  while ((dp = readdir(dir)) != NULL) {
    if (dp != NULL) {
      if (strcmp(dp->d_name, ".") && strcmp(dp->d_name, "..")) {
        get_info(&files[curr++], dirName, dp->d_name);
      }
    } else {
      perror("");
      exit(1);
    }
  }

  qsort(files, curr, sizeof(info*), mycmp);

  int i;
  for (i = 0; i < curr; ++i) {
    info *f = files[i];      
    printf("%s %ld %s %s %ld %s\n", f->atrr, f->count_link, f->owner, f->group, f->size, f->name);
  }

  for (i = 0; i < curr; ++i) {
    info *f = files[i];
    if (f->atrr[0] == 'd') {
      DIR *d = opendir(f->path);
      if (d != NULL) {
        printf("\n%s:\n", f->path);
        ls(d, f->path);
        closedir(d);
      } else {
        perror(f->path);
      }
    }
    free(f);
  }
}

int main(int argc, const char *argv[]) {
  const char *s;

  if (argc < 2) {
    s = ".";
  } else {
    s = argv[1];
  }

  DIR *dir = opendir(s);
  if (dir != NULL) {
    printf("%s:\n", s);
    ls(dir, s);
    closedir(dir);
  } else {
    info *f;
    get_info(&f, ".", s);
    printf("%s %ld %s %s %ld %s\n", f->atrr, f->count_link, f->owner, f->group, f->size, f->name);
    free(f);
  }
  return 0;
}
