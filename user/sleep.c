#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int n;

  if(argc < 2)
  {
    fprintf(2, "Usage: sleep number_of_seconds...\n");
    exit(1);
  }

  n = atoi(argv[1]);

  // 1 second is 10 clock ticks
  if(sleep(n * 10) < 0)
    fprintf(2, "sleep failed\n");

  exit(0);
}