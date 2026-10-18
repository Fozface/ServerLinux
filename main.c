#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "helper.h"
#include "sensor.h"

int main(int argc, char *argv[]) {
  int chk = 0;
  // gets the file descriptorfile descriptor, which is a small,
  //  non-negative integer used in subsequent I/O operations

  if (argc != 2) {
    errno = EINVAL;
    perror("invalid argument for main");
    return 1;
  }
  char *endptr = 0;
  // if here = ;endptr doesnt = null then something other than a number
  // was entered, if  argc != 2 then no arg was entered or more than 1 arg
  // was entered, the file name is always arg 0. if endptr == argv[1] no
  // conversion was done by strtol
  long count = strtol(argv[1], &endptr, 10);
  if (!(*endptr == '\0') || (endptr == argv[1]) || (count < 0) ||
      (errno != 0)) {
    errno = (errno == 0) ? EINVAL : errno;
    perror("invalid argument for main");
    return 1;
  }
  long temp = 0;
  int fd;
  int bytes = 0;
  if (openTemp(&fd) != 0) {
    char tempCallError[] = "openTemp call failed\n";
    chk = checkWrite(
        write(STDERR_FILENO, tempCallError, sizeof(tempCallError) - 1),
        "openTemp");
    if (chk == 1)
      return 1;
  }
  for (int i = 0; i < count; i++) {
    int rd = readTemp(&temp, &bytes);
    if (rd == -1) {
      close(fd);
      return 1;
    }
    char output[10];
    if (bytes >= 9) {
      char outputbufferror[] = "read more bytes than output can handle\n";
      chk = checkWrite(
          write(STDERR_FILENO, outputbufferror, sizeof(outputbufferror) - 1),
          "bufSize");
      if (chk == 1)
        return 1;
    }

    int tempCwhole = (temp / 1000);
    int tempCdec = ((temp + 50) / 100) % 10;
    int pr = snprintf(output, (bytes + 2), "%d.%d C\n", tempCwhole, tempCdec);
    if (pr < 0) {
      perror("snprintf error of output");
      return 1;
    }
    chk = checkWrite(write(STDOUT_FILENO, output, pr), "printOut");
    if (chk == 1) {
      close(fd);
      return 1;
    }
    int ls = lseek(fd, 0, SEEK_SET);
    if (ls == -1) {
      perror("lseek error");
      return 1;
    } else if (ls != 0) {
      char lseekError[] = "lseek did not return to 0";
      chk = checkWrite(write(STDOUT_FILENO, lseekError, sizeof(lseekError) - 1),
                       "lseek");
      if (chk == 1)
        return 1;
    }
    sleep(1);
  }
  close(fd);
  return 0;
}
