/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/
#include "diskio.h"
#include "SD.h"

DSTATUS disk_initialize (BYTE drv)
{
	if(drv==0)
	{
		if(SD_Init())return STA_NOINIT;//初始化失败，返回未初始化
		else return 0;//初始化成功
	}else return STA_NODISK;//盘符不是SD卡，返回无媒体在此驱动器
}

DSTATUS disk_status (BYTE drv)
{
	return drv;
}

DRESULT disk_read (BYTE drv, BYTE *buff, DWORD sector, BYTE count)
{
	u8 i;
	if(drv)return RES_PARERR;//返回无效参数
	i=SD_ReadDisk(buff,sector,count);
	if(i)return RES_ERROR;
	else return RES_OK;
}

#if _READONLY == 0
DRESULT disk_write (BYTE drv, const BYTE *buff, DWORD sector, BYTE count)
{
	u8 i;
	if(drv)return RES_PARERR;//返回无效参数
	i=SD_WriteDisk((u8*)buff,sector,count);
	if(i)return RES_ERROR;
	else return RES_OK;
}
#endif

DRESULT disk_ioctl (BYTE drv, BYTE ctrl, void *buff)
{
	if(drv==0)
	{
		switch(ctrl)
		{
			case GET_SECTOR_SIZE:*(u32*)buff=512;break;
			case GET_SECTOR_COUNT:*(u32*)buff=((SD_GetCapacity()/512)-1);break;
			case GET_BLOCK_SIZE:*(u32*)buff=512;break;
		}
	}
	return RES_OK;	
}

DWORD get_fattime(void)
{
	DWORD temp=0;
// 	Get_Time();
// 	temp|=((time.year-1980)<<25);
// 	temp|=(time.month<<21);
// 	temp|=(time.day<<16);
// 	temp|=(time.hour<<11);
// 	temp|=(time.minutes<<5);
// 	temp|=(time.secon/2);
	return temp;
}        


