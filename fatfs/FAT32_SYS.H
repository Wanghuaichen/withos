#ifndef _FAT32_H_
#define _FAT32_H_
#include "stm32f10x.h"

/******************************声明文件属性结构体*********************************/
struct file_sys
{
  u8  file_name[8];	      //文件名
  u8  file_kuozhan[3];    //扩展名
  u32 file_size;          //文件大小(字节)
//  u32 file_start_add_b; //文件自身起始地址基址
//  u16 file_start_add_p; //文件自身起始地址偏移址
  u32 file_content_add;	  //文件实体内容的地址
  u8  file_type;	      //文件类型(文件夹/文本……)
/*
*************************************************************************************************
***  file_type的值是自定义的：
***  0--未定义、1--文件夹、2--文本文档、3--BIN格式、4--MP3格式、5--BMP格式、6--MID格式、7--其他
*************************************************************************************************/
};
struct MP3_List
{
  u8 mp3_name[8];	      //文件名
  u32 mp3_size;           //文件大小(字节)
//u32 mp3_start_add_b;    //文件自身起始地址基址
//u16 mp3_start_add_p;    //文件自身起始地址偏移址
  u32 mp3_content_add;	  //文件实体内容的地址
  u8 mp3_number;
};
extern u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt);	

// extern void MyPrintf(u8 *Data);

void Get_Data_Start_Add(void);

#endif
