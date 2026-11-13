#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "helper.h"

int checkWrite(int wr, char *from) {
  if (wr == -1) {
    perror(from);
    return 1;
  }
  return 0;
}

int getOp(int argc, char *argv[], long num, long sec, char out, long zone) {
  int flags, op = 0;
  num = 0;
  sec = 0;
  char *endptr;

  while ((op = getopt(argc, argv, "niozh")) != -1) {

    switch (op) {
    case 'n':
      // number of times to run tempmon
      errno = 0;
      num = strtol(optarg, &endptr, 10);
      if (!(*endptr == '\0') || (endptr == argv[1]) || (num < 0) ||
          (errno != 0)) {
        errno = (errno == 0) ? EINVAL : errno;
        perror("invalid argument for main");
        return 1;
      }

      break;

    case 'i':
      break;
    case 'o':
      break;
    case 'z':
      break;
    case 'h':
      break;
    default:
      break;
    }
  }
  return flags;
}
