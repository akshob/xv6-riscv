#include <stddef.h>
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
getname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void
find(char *path, char *name) {
  char buf[512];
  char *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type) {
    case T_FILE:
      if (name == NULL) {
        printf("%s\n", path);
      }
      else if (strcmp(name, getname(path)) == 0) {
        printf("%s\n", path);
      }
      close(fd);
      break;

    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
          printf("find: cannot stat %s\n", buf);
          continue;
        }
        char * dirname = getname(buf);
        if (strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0) {
          continue;
        }
        find(buf, name);
      }
      break;
  }
}

int
main(int argc, char *argv[])
{
  if (argc < 2) {
    fprintf(2, "usage: find path [name]\n");
    exit(0);
  }

  if(argc < 3){
    find(argv[1], NULL);
    exit(0);
  }

  find(argv[1], argv[2]);
  exit(0);
}