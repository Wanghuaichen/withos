#include "Touch_s.h"

u16 TP_X,TP_Y;		   
/****************************说明************************/
//经测试，触摸屏四角坐标值（x,y）分别是：
//左上：（130,115 ） 右上：（1880,115 ）
//左下：（130,1850） 右下：（1880,1850）
//山寨图标最底边Y=1990
void delays(u16 d)
{
 while(d--);
}
void Touch_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  |
								                GPIO_Pin_1  |
								                GPIO_Pin_2  ;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;		   // 推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   // 最高输出速率50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;	      
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //复用开漏输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//**********************************************************
void WriteCharTo7843(u8 num)          //SPI写数据
{
u8 count=0;
DCLK_0;
for(count=0;count<8;count++)
{
if((num&0x80)==0x00)
DIN_0;
else
DIN_1;
num<<=1;                //上升沿有效
DCLK_0;                 //上升沿有效
delays(1);
DCLK_1; 
}
}
//**********************************************************
u16 ReadFromCharFrom7843(void)             //SPI 读数据
{
u8 count=0;
u16 Num=0;
for(;count<12;count++)
{
Num<<=1;
DCLK_1;
delays(1);
DCLK_0;
if(DOUT!=0) 
Num++;
}
return(Num);
}
