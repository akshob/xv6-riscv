#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void redirectedPipes();
void redirect(int, int[]);
void receiver();
void worker(int);

int
main(int argc, char *argv[])
{
  redirectedPipes();
  exit(0);
}

void
redirectedPipes() {
  int pd[2], n;
  pipe(pd);
  if (fork() == 0) {
    redirect(1, pd);
    for (n = 2; n <= 35; n++) {
      write(1, &n, sizeof(n));
    }
  } else {
    redirect(0, pd);
    receiver();
  }
}

void
receiver() {
  int p;
  if(read(0, &p, sizeof(p))) {
    printf("prime %d\n", p);

    int workerPipe[2];
    pipe(workerPipe);

    if(fork() == 0) {
      redirect(1, workerPipe);
      worker(p);
    } else {
      redirect(0, workerPipe);
      receiver();
    }
  }
}

void
worker(int p) {
  int n;
  while(read(0, &n, sizeof(n))) {
    if (n % p != 0) {
      write(1, &n, sizeof(n));
    }
  }
}

void redirect(int k, int communicator[]) {
  close(k);
  dup(communicator[k]);
  close(communicator[0]);
  close(communicator[1]);
}