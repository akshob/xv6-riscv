#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  int pid;
  char buf;
  pipe(p);
  if (fork() == 0) { //child
    pid = getpid();
    read(p[0], &buf, 1);
    close(p[0]);
    printf("%d: received ping\n", pid);
    write(p[1], (void *)'p', 1);
    close(p[1]);
  } else { //parent
    pid = getpid();
    write(p[1], (void *)'c', 1);
    close(p[1]);
    read(p[0], &buf, 1);
    close(p[0]);
    printf("%d: received pong\n", pid);
  }
  exit(0);
}

