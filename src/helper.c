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

int parseOp(int argc, char *argv[], long *num, long *sec, char *out, long *zone) {
  int exitVal = 0, op = 0;
  *num = 0;
  *sec = 1;
  char *endptr;

  while ((op = getopt(argc, argv, "niozh")) != -1) {

    switch (op) {
    case 'n':
      // number of times to run tempmon
      errno = 0;
      *num = strtol(optarg, &endptr, 10);
      if (!(*endptr == '\0') || (endptr == argv[1]) || (*num < 0) ||
          (errno != 0)) {
        errno = (errno == 0) ? EINVAL : errno;
        perror("invalid argument for main");
        return 1;
      }

      break;

    case 'i':
        // seconds between readings, min is 1, 
    errno = 0;
      *sec = strtol(optarg, &endptr, 10);
      if (!(*endptr == '\0') || (endptr == argv[1]) || (*sec < 1) ||
          (errno != 0)) {
        errno = (errno == 0) ? EINVAL : errno;
        perror("invalid argument for main");
        return 1;
      }
        break;

    case 'o':
        //make out point to the address of the output argument
        *out = *optarg;
       break;

    case 'z':
     errno = 0;
      *zone = strtol(optarg, &endptr, 10);
      if (!(*endptr == '\0') || (endptr == argv[1]) || (*zone < 0) ||
          (errno != 0)) {
        errno = (errno == 0) ? EINVAL : errno;
        perror("invalid argument for main");
        return 1;
      }
        break;

    case 'h':
       exitVal = 2; 
      break;
    default:
      break;
    }
  }
  return exitVal;
}
