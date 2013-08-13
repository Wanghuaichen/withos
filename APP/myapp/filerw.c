#include "delay.h"
#include "flash.h"
#include "edit.h"
#include"gui.h"
#include"wm.h"
#include <string.h>
#include "motordrive.h"
#include "filerw.h"

#define StartByte (sizeof(unsigned int))

#define FLASH_SIZE (8*1024*1024)

//char fnamebuf[30];
//char fileReadBuf[fileReadBufMax];//可以每当touch_child发生之后就把数据读到filereadbuf，当ok之后把数据写入flash，前三组放入下面数组

char curModeName[ModeNameLenMax];
char curSpeed
//三组mode
char modeName1[ModeNameLenMax];
char modeName2[ModeNameLenMax];
char modeName3[ModeNameLenMax];

unsigned speed1[maxstep];
unsigned speed2[maxstep];
unsigned speed3[maxstep];
unsigned duration1[maxstep];
unsigned duration2[maxstep];
unsigned duration3[maxstep];

#define ByteInAGroup (ModeNameLenMax + fileReadBufMax)
unsigned int getCount()
{
		//大端小端？？？
		unsigned int cnt;
		SPI_Flash_Read((char*)&cnt, 0, sizeof(unsigned int));
		return cnt;
	
}
void setCount(unsigned int cnt)
{
		SPI_Flash_Write((char*)&cnt, 0, sizeof(unsigned int));
}

unsigned char readData(unsigned int *speedarr, unsigned int *durationarr, unsigned index)
{
		unsigned addr = StartByte + index * ( fileReadBufMax );
		if(index >= getCount()){
				return 0;
		}	
		SPI_Flash_Read((char*)speedarr, addr, fileReadBufMax / 2);
		SPI_Flash_Read((char*)durationarr, addr+fileReadBufMax / 2, fileReadBufMax / 2);
		return 1;
}

unsigned char writeData(unsigned int *speedarr, unsigned int *durationarr)
{		
		unsigned cnt = getCount();
		unsigned addr = StartByte + cnt++ * fileReadBufMax;
	
		SPI_Flash_Write((char*)speedarr, addr, fileReadBufMax / 2);
		SPI_Flash_Write((char*)durationarr, addr+fileReadBufMax / 2, fileReadBufMax / 2);
	
		SPI_Flash_Write((char*)&cnt, 0, (sizeof(unsigned)));
		return 1;
}

void swapData(unsigned index1, unsigned index2)
{
		unsigned char buf[fileReadBufMax];
		readData((unsigned*)buf, (unsigned*)(buf+fileReadBufMax/2), index1);
		readData((unsigned*)fileReadBuf, (unsigned*)(fileReadBuf+fileReadBufMax/2), index2);
	
		SPI_Flash_Write((char*)fileReadBuf, StartByte + index1 * fileReadBufMax, fileReadBufMax / 2);
		SPI_Flash_Write((char*)(fileReadBuf+fileReadBufMax / 2), StartByte + index1 * fileReadBufMax + fileReadBufMax / 2, fileReadBufMax / 2);
	
		SPI_Flash_Write((char*)buf, StartByte + index2 * fileReadBufMax, fileReadBufMax / 2);
		SPI_Flash_Write((char*)(buf+fileReadBufMax / 2), StartByte + index2 * fileReadBufMax + fileReadBufMax / 2, fileReadBufMax / 2);	
}

unsigned initData(void)
{
		unsigned cnt = getCount(), i = 0;
		if(cnt == 0){
				return 0;
		}
		readData(speed1, duration1, 0);
		if(1 == cnt){
				return 1;
		}
		readData(speed2, duration2, 1);
		if(2 == cnt){
				return 2;
		}	
		readData(speed3, duration3, 1);
		return 3;
}
/*
void getFileList(char **fnamelist)
{
		
}
void retrieveMode(char *fname)
{
		
}

unsigned retrueveName(unsigned char index)
{
		char teststr[] = "teststring";
		unsigned int len = strlen(teststr);
		SPI_Flash_Write(teststr,FLASH_SIZE-len*2,len);
		delay_ms(100);
		SPI_Flash_Read(fileReadBuf,FLASH_SIZE-len*2,len);
		fileReadBuf[len] = 0;
		return 0;
}

void test(EDIT_Handle hwin)
{
		unsigned int i = 0;
//		i = retrueveName(1);
	//	EDIT_SetText(hwin, fileReadBuf);
		SPI_Flash_Write((char*)&i, 0, (sizeof(unsigned)));
}
*/
