#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "helper.h"
#include "sensor.h"

static int FD = -1;
static int WR = -1;
static int RD = -1;

int openTemp(int *fd)
{

	FD = open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);
	if (FD == -1) {
		perror("open error in openTemp()");
		return -1;
	}
	*fd = FD;
	return 0;
}

int readTemp(long *tempL, int *rd)
{

	char buffer[10] = {};

	if (FD == -1) {
		char readError[] = "no file was opened\n";
		WR = write(STDERR_FILENO, readError, sizeof(readError) - 1);
		if (WR == -1) {
			perror("write error in readTemp()");
			return -1;
		}
		return -1;
	}
	RD = read(FD, buffer, sizeof(buffer) - 1);
	if (RD == -1) {
		perror("read error in readTemp");
		return -1;
	}
	*rd = RD;
	if (RD == sizeof(buffer) - 1) {
		char buferror[] =
		    "the buffer may be to small to get all data\n";
		WR = write(STDERR_FILENO, buferror, sizeof(buferror) - 1);
		if (WR == -1) {
			perror("write error in readTemp buferror");
			return -1;
		}
	}
	char *endarr = 0;
	*tempL = strtol(buffer, &endarr, 10);
	if (!(*endarr == '\n') || (endarr == buffer) || (*tempL < 0)) {
		errno = (errno == 0) ? EINVAL : errno;
		perror("strtol error in readTemp");
		return -1;
	}

	return 0;
}
