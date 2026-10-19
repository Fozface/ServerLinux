#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "helper.h"
#include "sensor.h"

/*returning 1 should be done frome main, that indicate my function failed.
 * return -1 comes from system functions  */
int main(int argc, char *argv[])
{
	int wr, bytes, chk, setStart, pr, tempInt, tempDec;

	long temp = 0;
	long count;

	char *endptr = 0;
	char output[10];

	if (argc != 2) {
		errno = EINVAL;
		perror("main requires a int to run\n");
		return 1;
	}
	// if here = ;endptr doesnt = null then something other than a number
	// was entered, if  argc != 2 then no arg was entered or more than 1 arg
	// was entered, the file name is always arg 0. if endptr == argv[1] no
	// conversion was done by strtol
	errno = 0;
	count = strtol(argv[1], &endptr, 10);
	if (!(*endptr == '\0') || (endptr == argv[1]) || (count < 0) ||
	    (errno != 0)) {
		errno = (errno == 0) ? EINVAL : errno;
		perror("invalid argument for main");
		return 1;
	}

	if (openTemp() != 0) {
		char tempCallEr[] = "openTemp call failed\n";
		wr = write(STDERR_FILENO, tempCallEr, sizeof(tempCallEr) - 1);
		chk = checkWrite(wr, "openTemp\n");
    return 1;
	}
	for (int i = 0; i < count; i++) {
		if (readTemp(&temp, &bytes)) {
			return 1;
		}

		if (bytes >= (int)sizeof(output)) {
			char outbuff[] = "read bytes > output size\n";
			wr = write(STDERR_FILENO, outbuff, sizeof(outbuff) - 1);
			chk = checkWrite(wr, "bufSize\n");
			if (chk == 1) {
				closeTemp();
				return 1;
			}
			closeTemp();
			return 1;
		}
		tempInt = ((temp + 50) / 1000);
		tempDec = ((temp + 50) / 100) % 10;

		pr = snprintf(output, sizeof(output), "%d.%d C\n", tempInt, tempDec);

		if (pr >= (int)sizeof(output)) {
      pr = sizeof(output) - 1;
			char snpErr[] = "\nsnprintf truncated output\n";
			wr = write(STDERR_FILENO, snpErr, sizeof(snpErr) - 1);
			chk = checkWrite(wr, "snprtf\n");
			if (chk == 1) {
				closeTemp();
				return 1;
			}
		}
		wr = write(STDOUT_FILENO, output, pr);
		chk = checkWrite(wr, "printOut\n");
		if (chk == 1) {
			closeTemp();
			return 1;
		}

		setStart = setReadStart(0);
		if (setStart != 0) {
			return 1;
		}
		sleep(1);
	}
	closeTemp();
	return 0;
}
