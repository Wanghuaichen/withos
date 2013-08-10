#ifndef _motordrive
#define _motordrive
//global data 
#define maxstep 20
extern unsigned step[maxstep];
extern unsigned speed[maxstep];
void runMode(unsigned char n);
unsigned int getStatus(unsigned char modeindex);
#endif