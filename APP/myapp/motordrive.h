#ifndef _motordrive
#define _motordrive
//global data 
#define maxstep 10
extern unsigned duration[maxstep];
extern char lastfname[30];
extern unsigned speed[maxstep];
void runMode(unsigned char n);
unsigned int getStatus(unsigned char modeindex);
#endif