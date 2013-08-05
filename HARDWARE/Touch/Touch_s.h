#ifndef __Touch_s_H
#define __Touch_s_H
#include "stm32f10x.h"

#define T_DATA     GPIOC
#define DCLK_0	   T_DATA->BRR  = GPIO_Pin_0
#define DCLK_1	   T_DATA->BSRR = GPIO_Pin_0
#define TCS_0      T_DATA->BRR  = GPIO_Pin_1
#define TCS_1      T_DATA->BSRR = GPIO_Pin_1
#define DIN_0	     T_DATA->BRR  = GPIO_Pin_2
#define DIN_1	     T_DATA->BSRR = GPIO_Pin_2

#define DOUT  	  (T_DATA->IDR  & GPIO_Pin_3)

// #define Touch_IRQ (T_DATA->IDR & GPIO_Pin_4)

void Touch_GPIO_Init(void);
void delays(u16 d);
void WriteCharTo7843(u8 num);
u16  ReadFromCharFrom7843(void);

#endif
