#ifndef _filerw
#define _filerw
#include "ff.h"

#define maxstep 10

//#define fileReadBufMax (maxstep*(sizeof(unsigned))*2)
//extern FIL theFile;
//extern char fnamebuf[30];
#define ModeNameLenMax 30
extern char curModeNameBuf_test[ModeNameLenMax];
extern unsigned curDuration_test[maxstep];
extern unsigned curSpeed_test[maxstep];
extern char curModeNameBuf[ModeNameLenMax];
extern unsigned curDuration[maxstep];
extern unsigned curSpeed[maxstep];

extern char modeName1[ModeNameLenMax];
extern char modeName2[ModeNameLenMax];
extern char modeName3[ModeNameLenMax];

extern unsigned speed1[maxstep];
extern unsigned speed2[maxstep];
extern unsigned speed3[maxstep];
extern unsigned duration1[maxstep];
extern unsigned duration2[maxstep];
extern unsigned duration3[maxstep];

unsigned getInt(unsigned int addr);
void setInt(unsigned int addr, unsigned dat);
unsigned int getCount(void);
void setCount(unsigned int cnt);
unsigned char readData(char *modename, unsigned int *speedArr, unsigned int *durationArr, unsigned index);
unsigned char writeData(char *modename, unsigned int *speedArr, unsigned int *durationArr);
void editData(char *modename, unsigned int *speedArr, unsigned int *durationArr, unsigned index);
void deleteData(unsigned index);
void swapGroupIndexBuf(unsigned index1, unsigned index2);
void refreshGroupIndex(void);
unsigned initData(void);
void clearFlash(void);
void swapData(unsigned index1, unsigned index2);

void writeToFile(void *data, unsigned len, char *fname);
#endif