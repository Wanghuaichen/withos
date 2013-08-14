#include "delay.h"
#include "flash.h"
#include "edit.h"
#include"gui.h"
#include"wm.h"
#include <string.h>
#include "filerw.h"



//char fnamebuf[30];
//char fileReadBuf[fileReadBufMax];//可以每当touch_child发生之后就把数据读到filereadbuf，当ok之后把数据写入flash，前三组放入下面数组
unsigned groupIndex[MaxNumOfGroups];
unsigned groupIndexCounter = 0;

char curModeNameBuf_test[ModeNameLenMax] = "mode name test";
unsigned curDuration_test[maxstep] = {1,2,3,4,5,6,7,8,9,10};
unsigned curSpeed_test[maxstep] = {1,2,3,4,5,6,7,8,9,10};
char curModeNameBuf[ModeNameLenMax];
unsigned curDuration[maxstep];
unsigned curSpeed[maxstep];
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

unsigned getInt(unsigned int addr)
{
		unsigned res;
		SPI_Flash_Read((char*)&res, addr, sizeof(unsigned int));
		return res;
}

void setInt(unsigned int addr, unsigned dat)
{
		SPI_Flash_Write((char*)&dat, addr, sizeof(unsigned int));
}

unsigned int getCount(void)
{
		return getInt(0);	
}

void setCount(unsigned int cnt)
{
		setInt(0, cnt);
}
unsigned getAddr(unsigned index)
{
		return getInt(AddrGroupIndex + index*(sizeof(unsigned)));
}
unsigned char readData(char *modename, unsigned int *speedArr, unsigned int *durationArr, unsigned index)
{
		unsigned addr;
		if(index >= getCount()){
				return 0;
		}	
		addr = getAddr(index);//get the absolute address
		SPI_Flash_Read((char*)modename, addr, ModeNameLenMax);
		SPI_Flash_Read((char*)speedArr, addr+ModeNameLenMax, maxstep * (sizeof(unsigned)));
		SPI_Flash_Read((char*)durationArr, addr+ModeNameLenMax+maxstep * (sizeof(unsigned)), maxstep * (sizeof(unsigned)));
		return 1;
}

unsigned char writeData(char *modename, unsigned int *speedArr, unsigned int *durationArr)
{		
		//unsigned cnt = getCount();
		//groupIndexCounter is initiated during startup in initData()
		unsigned addr = AddrData + groupIndexCounter * BytesPerGroup;
		if(groupIndexCounter >= MaxNumOfGroups){
				return 0;
		}
		SPI_Flash_Write((char*)modename, addr, ModeNameLenMax);
		SPI_Flash_Write((char*)speedArr, addr+ModeNameLenMax, maxstep * (sizeof(unsigned)));
		SPI_Flash_Write((char*)durationArr, addr+ModeNameLenMax+maxstep * (sizeof(unsigned)), maxstep * (sizeof(unsigned)));
		
		groupIndex[groupIndexCounter++] = addr;

		return 1;
}
void editData(char *modename, unsigned int *speedArr, unsigned int *durationArr, unsigned index)
{		
		unsigned addr = groupIndex[index];
		SPI_Flash_Write((char*)modename, addr, ModeNameLenMax);
		SPI_Flash_Write((char*)speedArr, addr+ModeNameLenMax, maxstep * (sizeof(unsigned)));
		SPI_Flash_Write((char*)durationArr, addr+ModeNameLenMax+maxstep * (sizeof(unsigned)), maxstep * (sizeof(unsigned)));
}

void deleteData(unsigned index)
{
		unsigned int i;
		for(i = index; i < groupIndexCounter - 1; ++i){
				groupIndex[i] = groupIndex[i+1];
		}
		--groupIndexCounter;
}

void swapGroupIndexBuf(unsigned index1, unsigned index2)
{
		groupIndex[index1] ^= groupIndex[index2];
		groupIndex[index2] ^= groupIndex[index1];
		groupIndex[index1] ^= groupIndex[index2];
}

void refreshGroupIndex(void)
{
		SPI_Flash_Write((char*)groupIndex, AddrGroupIndex, MaxNumOfGroups*(sizeof(unsigned)));
		setCount(groupIndexCounter);
}

void clearFlash(void)
{
		unsigned i = 0;
		SPI_Flash_Write((char*)&i, 0, sizeof(unsigned));
		groupIndexCounter = 0;
}
/*void swapData(unsigned index1, unsigned index2)
{
		unsigned tmp1, tmp2;
		tmp1 = getInt(AddrGroupIndex + index1);
		tmp2 = getInt(AddrGroupIndex + index2);
		setInt(AddrGroupIndex + index1, tmp2);
		setInt(AddrGroupIndex + index2, tmp1);
}*/
/*void swapData(unsigned index1, unsigned index2)
{
		unsigned char buf[fileReadBufMax];
		readData((unsigned*)buf, (unsigned*)(buf+fileReadBufMax/2), index1);
		readData((unsigned*)fileReadBuf, (unsigned*)(fileReadBuf+fileReadBufMax/2), index2);
	
		SPI_Flash_Write((char*)fileReadBuf, StartByte + index1 * fileReadBufMax, fileReadBufMax / 2);
		SPI_Flash_Write((char*)(fileReadBuf+fileReadBufMax / 2), StartByte + index1 * fileReadBufMax + fileReadBufMax / 2, fileReadBufMax / 2);
	
		SPI_Flash_Write((char*)buf, StartByte + index2 * fileReadBufMax, fileReadBufMax / 2);
		SPI_Flash_Write((char*)(buf+fileReadBufMax / 2), StartByte + index2 * fileReadBufMax + fileReadBufMax / 2, fileReadBufMax / 2);	
}*/
extern char * _aTable_1[3][2];
unsigned initData(void)
{
		unsigned cnt = getCount(), i = 0;
		groupIndexCounter = cnt;
		if(cnt == 0){
				return 0;
		}
		SPI_Flash_Read((char*)groupIndex, AddrGroupIndex, MaxNumOfGroups*(sizeof(unsigned)));
		
		readData(modeName1, speed1, duration1, 0);
		_aTable_1[0][0] = "1";
		_aTable_1[0][1] = modeName1;
		if(1 == cnt){
				return 1;
		}
		readData(modeName2, speed2, duration2, 1);
		_aTable_1[1][0] = "2";
		_aTable_1[1][1] = modeName2;		
		if(2 == cnt){
				return 2;
		}	
		readData(modeName3, speed3, duration3, 2);
		_aTable_1[2][0] = "3";
		_aTable_1[2][1] = modeName3;		

		i = 3;
		return cnt;
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
