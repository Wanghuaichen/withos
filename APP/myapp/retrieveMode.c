#include "delay.h"
#include "flash.h"
#include "edit.h"
#include"gui.h"
#include"wm.h"
#include <string.h>
#include "ff.h"
#include "motordrive.h"
#include "filerw.h"

#define StartByte (sizeof(unsigned int))

#define FLASH_SIZE (8*1024*1024)

FIL theFile;
char fnamebuf[30];
char fileReadBuf[fileReadBufMax];


unsigned int getCount()
{
		//´ó¶ËÐ¡¶Ë£¿£¿£¿
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
		unsigned addr = StartByte + getCount() * fileReadBufMax;
	
		SPI_Flash_Write((char*)speedarr, addr, fileReadBufMax / 2);
		SPI_Flash_Write((char*)durationarr, addr+fileReadBufMax / 2, fileReadBufMax / 2);
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
		i = retrueveName(1);
		EDIT_SetText(hwin, fileReadBuf);
}
