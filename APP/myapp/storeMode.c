#include "filerw.h"

void writeToFile(void *data, unsigned len, char *fname)
{
		int i;
		f_open(&theFile, fname, FA_WRITE);
		f_write(&theFile, fname, len, &i);
		f_close(&theFile);
}