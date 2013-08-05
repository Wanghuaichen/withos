#ifndef _vs1003_h_
#define _vs1003_h_
#include "stm32f10x.h"
/**************************************************************/

#define VS_DATA GPIOC	  //E7--E13

#define TCLK_0  VS_DATA->BRR = GPIO_Pin_11
#define TCLK_1  VS_DATA->BSRR = GPIO_Pin_11

#define TDIN_0  VS_DATA->BRR = GPIO_Pin_6	//接vs1003的MOSI
#define TDIN_1  VS_DATA->BSRR = GPIO_Pin_6

#define TXCS_0  VS_DATA->BRR = GPIO_Pin_8
#define TXCS_1  VS_DATA->BSRR = GPIO_Pin_8 

#define TXDCS_0 VS_DATA->BRR = GPIO_Pin_9
#define TXDCS_1 VS_DATA->BSRR = GPIO_Pin_9

#define TRST_0  VS_DATA->BRR = GPIO_Pin_12
#define TRST_1  VS_DATA->BSRR = GPIO_Pin_12 

#define TDOUT  (VS_DATA->IDR & GPIO_Pin_7)   //PE7数据输入 接vs1003的MISO
#define DREQ   (VS_DATA->IDR & GPIO_Pin_10)

/**************************************************************/	

#define VS_WRITE_COMMAND 	0x02
#define VS_READ_COMMAND 	0x03		 		 

#define SPI_MODE        	0x00   
#define SPI_STATUS      	0x01   
#define SPI_BASS        	0x02   
#define SPI_CLOCKF      	0x03   
#define SPI_DECODE_TIME 	0x04   
#define SPI_AUDATA      	0x05   
#define SPI_WRAM        	0x06   
#define SPI_WRAMADDR    	0x07   
#define SPI_HDAT0       	0x08   
#define SPI_HDAT1       	0x09   
#define SPI_AIADDR      	0x0a   
#define SPI_VOL         	0x0b   
#define SPI_AICTRL0     	0x0c   
#define SPI_AICTRL1     	0x0d   
#define SPI_AICTRL2     	0x0e   
#define SPI_AICTRL3     	0x0f   
#define SM_DIFF         	0x01   
#define SM_JUMP         	0x02   
#define SM_RESET        	0x04   
#define SM_OUTOFWAV     	0x08   
#define SM_PDOWN        	0x10   
#define SM_TESTS        	0x20   
#define SM_STREAM       	0x40   
#define SM_PLUSV        	0x80   
#define SM_DACT         	0x100   
#define SM_SDIORD       	0x200   
#define SM_SDISHARE     	0x400   
#define SM_SDINEW       	0x800   
#define SM_ADPCM        	0x1000   
#define SM_ADPCM_HP     	0x2000 

#define MP3CMD_InitVS1003	0x11
#define MP3CMD_Play				0x12
#define MP3CMD_Pause			0x13
#define MP3CMD_Stop				0x14
#define MP3CMD_Next				0x15
#define MP3CMD_TestVS1003	0x16

/********************************************************************/
#define DefaultVol  65

void VS1003_GPIO_Init(void);
void Mp3Reset(void);
void Mp3SetVolume(u8 leftchannel,u8 rightchannel);
void Mp3SoftReset(void);
void Mp3WriteRegister(u8 addressbyte,u8 highbyte,u8 lowbyte);
u16 Mp3ReadRegister(u8 addressbyte);
void VsSineTest(void);
void WriteByteADS(u8 num);
u16 ReadWordADS(void);
void Delaya(u16 a);
void MP3Play(void);
void Get_mp3_info(void);
void Get_wave(void);
void LoadPatch(void);

void VS_Set_Bands(u16 *buf,u8 bands);
//void Clear_vs1003(void);
#endif

