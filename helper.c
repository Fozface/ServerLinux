#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "helper.h"

int checkWrite(int wr, char from[]) {
  if (wr == -1) {
    perror(from);
    return 1;
  }
  return 0;
}






