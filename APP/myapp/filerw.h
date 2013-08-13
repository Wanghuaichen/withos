#ifndef _filerw
#define _filerw
#include "ff.h"
#define bytesInInt (sizeof(int))
#define fileReadBufMax (maxstep*bytesInInt*2)
extern FIL theFile;
extern char fnamebuf[30];

void writeToFile(void *data, unsigned len, char *fname);
unsigned int getCount();
void setCount(unsigned int cnt);
unsigned char readData(unsigned int *speedarr, unsigned int *durationarr, unsigned index);
unsigned char writeData(unsigned int *speedarr, unsigned int *durationarr);
void swapData(unsigned index1, unsigned index2);


#endif