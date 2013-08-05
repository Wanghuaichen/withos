#ifndef __LCD_9320_H
#define __LCD_9320_H

#include"stm32f10x.h"
#define Bank1_LCD_D    ((uint32_t)0x60020000)  //disp Data ADDR
#define Bank1_LCD_C    ((uint32_t)0x60000000)	 //disp Reg ADDR

#define RES_0 GPIOA->BRR  = GPIO_Pin_0
#define RES_1 GPIOA->BSRR = GPIO_Pin_0

#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

//====================================================//
u16 ABS(s16 a);//求取绝对值
void LCD9320_GPIO_Init(void);
void ili9320_Init(void);
void ili9320_Initializtion(void);
void Init_data(u16 x,u16 y);
void Write_COM(u16 CMD);
void Write_DATA(u16 shuju);
u16 LCD_RD_data(void);
u16 ili9320_GetPoint(u16 x,u16 y);

void delayms(u16 tt);
void Address_set(u16 xx1,u16 yy1,u16 xx2,u16 yy2);
void Draw_Pixel(u16 x,u16 y,u16 color);
void draw_hline(s16 x0,s16 x1,u16 y,u16 color);
void draw_vline(s16 y0,s16 y1,u16 x,u16 color);

u16 ili9320_BGR2RGB(u16 c);
#endif
