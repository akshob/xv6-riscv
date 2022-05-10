#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void explicitPipes();
void receiver(int);
void worker(int, int, int);

int
main(int argc, char *argv[])
{
  explicitPipes();
  exit(0);
}

void
explicitPipes() {
  int pd[2], n;
  pipe(pd);
  if (fork() == 0) {
    close(pd[0]);
    for (n = 2; n <= 35; n++) {
      write(pd[1], &n, sizeof(n));
    }
  } else {
    close(pd[1]);
    receiver(pd[0]);
  }
}

void
receiver(int reader) {
  int p;
  if(read(reader, &p, sizeof(p))) {
    printf("prime %d\n", p);

    int workerPipe[2];
    pipe(workerPipe);

    if(fork() == 0) {
      close(workerPipe[0]);
      worker(p, reader, workerPipe[1]);
      close(workerPipe[1]);
    } else {
      close(workerPipe[1]);
      receiver(workerPipe[0]);
    }
  }
}

void
worker(int p, int reader, int writer) {
  int n;
  while(read(reader, &n, sizeof(n))) {
    if (n % p != 0) {
      write(writer, &n, sizeof(n));
    }
  }
}