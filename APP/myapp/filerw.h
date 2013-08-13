#ifndef _filerw
#define _filerw
#include "ff.h"
#define bytesInInt (sizeof(int))
#define fileReadBufMax (maxstep*bytesInInt*2)
extern FIL theFile;
extern char fnamebuf[30];
#define ModeNameLenMax 30
extern char modeNameBuf[ModeNameLenMax];


extern char modeName1[ModeNameLenMax];
extern char modeName2[ModeNameLenMax];
extern char modeName3[ModeNameLenMax];

extern unsigned speed1[maxstep];
extern unsigned speed2[maxstep];
extern unsigned speed3[maxstep];
extern unsigned duration1[maxstep];
extern unsigned duration2[maxstep];
extern unsigned duration3[maxstep];


void writeToFile(void *data, unsigned len, char *fname);
unsigned int getCount();
void setCount(unsigned int cnt);
unsigned char readData(unsigned int *speedarr, unsigned int *durationarr, unsigned index);
unsigned char writeData(unsigned int *speedarr, unsigned int *durationarr);
void swapData(unsigned index1, unsigned index2);
unsigned initData(void);

#endif