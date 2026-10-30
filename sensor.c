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

int openTemp()
{

	// /build/learn/w2/test.txt
	FD = open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);
	if (FD == -1) {
		perror("open error in openTemp()");
		return -1;
	}
	return 0;
}
void closeTemp()
{
	close(FD);
	FD = -1;
}

int readTemp(long *tempL, int *rd)
{
	static int wr;
	// static int chk;

	char buffer[8];

	if (FD == -1) {
		char readError[] = "no file was opened\n";
		WR = write(STDERR_FILENO, readError, sizeof(readError) - 1);
		if (WR == -1) {
			perror("write error in readTemp()");
			return -1;
		}
		return -1;
	}
	// read() trys to read (sizeof(buffer)-1) bytes from the open file at
	// file descriptor FD, and returns the number of bytes read,
	RD = read(FD, buffer, sizeof(buffer) - 1);
	if (RD == -1) {
		closeTemp();
		perror("read error in readTemp");
		return -1;
	}
	buffer[RD] = '\0';
	char *endarr = 0;
	errno = 0;
	*tempL = strtol(buffer, &endarr, 10);
	// catches overflow or underflow
	if (errno != 0) {
		perror("strtol error in readTemp");
		closeTemp();
		return -1;
	}
	// if it didnt reached the end of a string or there was nothing valid to
	// convert
	if ((*endarr != '\n') || (endarr == buffer)) {
		static char strtlEr[] =
		    "didnt reach end of string or nothing valid to convert\n";
		wr = write(STDERR_FILENO, strtlEr, sizeof(strtlEr) - 1);
		checkWrite(wr, "strtol");
		closeTemp();
		return 1;
	}
	*rd = RD;

	return 0;
}
int setReadStart(int start)
{

	off_t ls = lseek(FD, start, SEEK_SET);
	if (ls == -1) {
		closeTemp();
		perror("lseek error");
		return -1;
	} else if (ls != 0) {
		static int wr;
		static int chk;

		char lseekEr[] = "lseek did not return to 0\n";

		wr = write(STDERR_FILENO, lseekEr, sizeof(lseekEr) - 1);
		chk = checkWrite(wr, "lseek");
		if (chk == 1) {
			closeTemp();
			return 1;
		}
	}
	return 0;
}
