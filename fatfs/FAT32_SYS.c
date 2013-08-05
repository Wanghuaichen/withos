/********************************************************************************************************
FAT32文件系统的实现，功能较单一，只支持浏览、读出操作。
暂不支持文件的创建、删除、移动、复制等功能。

认为FAT32默认:每个簇的扇区数为0x08，每个扇区的字节数为0X200，并且所有文件均连续存储。
程序中出现的地址均为扇区地址，非物理地址。
说明：暂不支持长文件名显示
********************************************************************************************************/
#include "FAT32_SYS.H" 
#include <stdio.h>
#include "ff.h"
//#include"unicode_to_gb2312.h" //unicode转gb2312表，顺序排列，可以使用二分法查找

// u32 Keep_sector=0;          //保留扇区数
// u32 Data_start_add=0;       //数据区起始地址
// u32 FAT1_add=0;             //FAT1表的起始地址
// //u32 FAT2_add=0;           //FAT2表的起始地址
// u32 FAT_Block=0;            //每个FAT表占用的扇区数,位置：0X24--4 在此只用低16位，若TF卡容量超过2GB需使用32位

// extern FATFS fs; 

// /************获取数据区的起始地址和FAT1表的起始地址***************/
// void Get_Data_Start_Add(void)
// {
//   u32 stay_Block=0;           //保留扇区数，位置：0X0E--2
//   SD_ReadDisk(fs.win,0,1);
//   if(fs.win[3]==0x4d&&fs.win[4]==0x53)Keep_sector=0;     //没有保留扇区
//   else Keep_sector=(u32)fs.win[0x01c7]<<8|fs.win[0x01c6];//获得保留扇区数  
//   printf("Keep_sector = %d\r\n",Keep_sector);
// 	
// 	SD_ReadDisk(fs.win,Keep_sector+0,1);
//   printf((char *)fs.win);printf("\r\n");
// 	
//   FAT_Block=256*(u32)(fs.win[0x25])+(u32)(fs.win[0x24]);
//   stay_Block=256*(u32)(fs.win[0x0f])+(u32)(fs.win[0x0e]);
//   FAT1_add=stay_Block;
//  //FAT2_add=FAT_Block*512+FAT1_add;
//   Data_start_add=FAT1_add+FAT_Block*2;
// 	printf("Data_start_add = %d\r\n",Data_start_add);
// }  

