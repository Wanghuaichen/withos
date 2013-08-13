#ifndef _motordrive
#define _motordrive
#include "filerw.h"

//defined in filerw.h

//#define maxstep 10 
//extern unsigned curDuration[maxstep];
//extern unsigned curSpeed[maxstep];

void runMode(unsigned char n);
unsigned int getStatus(unsigned char modeindex);
#endif