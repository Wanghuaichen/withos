#include "vs1003.h"
#include "patch.h"
//extern u8 PHOTO[512];
//extern u8 SdReadBlock(u8 *Block, u32 address,u16 len);
u8 wave[14];
//u16 Play_time;
// u16 Bitrate,Frequency;//比特率、采样率
//u8 Vol=60;								 //音量调节255--0
//u8 BassEnhanceValue=0xf9/*0xfa*/,TrebleEnhanceValue=0x00;//重低音设置，90Hz以下增强15dB
void VS1003_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	 /**************************GPIOC******************************/	
  GPIO_InitStructure.GPIO_Pin = 
								GPIO_Pin_6  |
								GPIO_Pin_8  |
                GPIO_Pin_9  |	
								GPIO_Pin_11 |
								GPIO_Pin_12 ;			
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;		 // 推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 // 最高输出速率50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);				 // 选择E端口

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  |
                                GPIO_Pin_10 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //复用开漏输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/***************************************************/
void Delaya(u16 a)
{
  u16 b;
  for(;a!=0;a--)
  for(b=0;b!=1000;b++);
}
void WriteByteADS(u8 num)    
{  
	u8 count;    
	for(count=8;count;count--)  
	{ 	 
	  TCLK_0;//上升沿有效  
		if(num&0x80)
		TDIN_1;  
		else 
		TDIN_0;  
		num<<=1;    	 
		TCLK_1;      
	} 			    
}
u16 ReadWordADS(void)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 	 
	for(count=0;count<16;count++)  
	{ 	
     TCLK_1;  //下降沿有效		  
		 Num<<=1; 	 	   
		 TCLK_0; 
		 if(TDOUT)
		 Num|=1; 		 
	} 			  
	return(Num);   
}
 void Mp3Reset(void)
{	
    TRST_0;         //xReset = 0   复位vs1003();  
	Delaya(10);       
	WriteByteADS(0xff);//发送一个字节的无效数据，启动SPI传输
	TXDCS_1;       //xDCS =1
	TXCS_1;        //xCS = 1
	TRST_1;     
	TRST_0;       //xReset = 0   复位vs1003
	Delaya(100);
	TXCS_1;       //xCS = 1
	TXDCS_1;      //xDCS = 1
	TRST_1;       //xRESET = 1
	Delaya(100);
	while(DREQ==0);
	Delaya(100);
    Mp3SoftReset();//vs1003软复位
}
void Mp3SetVolume(u8 leftchannel,u8 rightchannel) 
{ 
  Mp3WriteRegister(SPI_VOL,leftchannel,rightchannel);
}
/**********************************************************/
/*  函数名称 :  Mp3SoftReset                              */
/*  函数功能 ： vs1003软件复位                            */
/*  参数     :  无                                        */
/*  返回值   :  无                                        */
/*--------------------------------------------------------*/
void Mp3SoftReset(void)
{
	Mp3WriteRegister (0,0x08,0x04); //软件复位
	Mp3WriteRegister (0,0x08,0x04); //软件复位
	Delaya(10);
    while(DREQ==0);  //等待软件复位结束
	Mp3WriteRegister(0x03, 0x98, 0x00);//设置vs1003的时钟,3倍频
    Mp3WriteRegister(0x03, 0x98, 0x00);//设置vs1003的时钟,3倍频
	Mp3WriteRegister (0x05, 0xBB, 0x81); //采样率48k，立体声
    Mp3WriteRegister (0x05, 0xBB, 0x81); //采样率48k，立体声
	Mp3WriteRegister(SPI_BASS, 0, 0);//设置重音
	Mp3WriteRegister(SPI_BASS, 0, 0);//设置重音
	Mp3SetVolume(DefaultVol,DefaultVol);//设置音量
	Mp3SetVolume(DefaultVol,DefaultVol);//设置音量
	Delaya(10);	
//  向vs1003发送4个字节无效数据，用以启动SPI发送
    TXDCS_0;     //xDCS = 0
	WriteByteADS(0);
	WriteByteADS(0);
	WriteByteADS(0);
	WriteByteADS(0);
	TXDCS_1;     //xDCS = 1
}
/*************************************************************/
/*  函数名称 :  Mp3WriteRegister                             */
/*  函数功能 ： 写vs1003寄存器                               */
/*  参数     :  寄存器地址，待写数据的高8位，待写数据的低8位 */
/*  返回值   :  无                                           */
/*-----------------------------------------------------------*/
void Mp3WriteRegister(u8 addressbyte, u8 highbyte, u8 lowbyte)
{
	TXDCS_1;       //xDCS = 1
	TXCS_0;       //xCS = 0
	WriteByteADS(VS_WRITE_COMMAND); //发送写寄存器命令
	WriteByteADS(addressbyte);      //发送寄存器的地址
	WriteByteADS(highbyte);         //发送待写数据的高8位
	WriteByteADS(lowbyte);          //发送待写数据的低8位
	TXCS_1;       //xCS = 1
}
/*************************************************************/
/*  函数名称 :  Mp3ReadRegister                              */
/*  函数功能 ： 写vs1003寄存器                               */
/*  参数     :  寄存器地址				     				 */
/*  返回值   :  vs1003的16位寄存器的值                       */
/*-----------------------------------------------------------*/
u16 Mp3ReadRegister(u8 addressbyte)
{
	u16 resultvalue = 0;//,ss;
	TXDCS_1;       //xDCS =1
	TXCS_0;       //xCS = 0
	WriteByteADS(VS_READ_COMMAND); //发送读寄存器命令
	WriteByteADS((addressbyte));//发送寄存器的地址
	resultvalue= ReadWordADS();  //读取低8位数据	  |
	TXCS_1;       //xCS = 1        
	return(resultvalue);                 //返回16位寄存器的值
}

/***********************************************************/
/*  函数名称 :  VsSineTest                                 */
/*  函数功能 ： vs1003正弦测试，将该函数放在while循环中，  */
/*              如果能持续听到一高一低的声音，证明测试通过 */                            
/*  参数     :  无                                         */
/*  返回值   :  无                                         */
/*---------------------------------------------------------*/
// void VsSineTest(void)
// {
// 	TRST_0;         //xReset = 0   复位vs1003();  
// 	Delaya(10);       
// 	WriteByteADS(0xff);//发送一个字节的无效数据，启动SPI传输
// 	TXDCS_1;       //xDCS =1
// 	TXCS_1;        //xCS = 1
// 	TRST_1;        //xReset =1 
// 	Mp3Reset();   
// 	Delaya(10);             
//     Mp3WriteRegister(SPI_MODE,0x08,0x20);//进入vs1003的测试模式
// 	//Mp3WriteRegister(SPI_MODE,0x08,0x00);  //进入vs1003的播放模式
// 	while(DREQ==0);
// }
/*************获得MP3文件的比特率、采样率等信息*********************/
// void Get_mp3_info(void)
// {
// 	u16 HDAT0,HDAT1,ID;
// 	HDAT0=Mp3ReadRegister(SPI_HDAT0);
// 	HDAT1=Mp3ReadRegister(SPI_HDAT1);
// 	ID=(HDAT1&0X0018)>>3;
// 	switch((HDAT0&0x0c00)>>10)
// 	{
// 	  case 0:if(ID==0||ID==1)Frequency=111;else if(ID==2)Frequency=220;else if(ID==3)Frequency=441;break;
// 	  case 1:if(ID==0||ID==1)Frequency=120;else if(ID==2)Frequency=240;else if(ID==3)Frequency=480;break;
// 	  case 2:if(ID==0||ID==1)Frequency=80; else if(ID==2)Frequency=160;else if(ID==3)Frequency=320;break;
// 	  default:Frequency=0;break;
// 	}
// 	switch((HDAT0&0xf000)>>12)
// 	{
// 	  case 1: if(ID==3)Bitrate=32;else Bitrate=8;break;
// 	  case 2: if(ID==3)Bitrate=40;else Bitrate=16;break;
// 	  case 3: if(ID==3)Bitrate=48;else Bitrate=24;break;
// 	  case 4: if(ID==3)Bitrate=56;else Bitrate=32;break;
// 	  case 5: if(ID==3)Bitrate=64;else Bitrate=40;break;
// 	  case 6: if(ID==3)Bitrate=80;else Bitrate=48;break;
// 	  case 7: if(ID==3)Bitrate=96;else Bitrate=56;break;
// 	  case 8: if(ID==3)Bitrate=112;else Bitrate=64;break;
// 	  case 9: if(ID==3)Bitrate=128;else Bitrate=80;break;
// 	  case 10:if(ID==3)Bitrate=160;else Bitrate=96;break;
// 	  case 11:if(ID==3)Bitrate=192;else Bitrate=112;break;
// 	  case 12:if(ID==3)Bitrate=224;else Bitrate=128;break;
// 	  case 13:if(ID==3)Bitrate=256;else Bitrate=144;break;
// 	  case 14:if(ID==3)Bitrate=320;else Bitrate=160;break;
// 	  default:Bitrate=0;break;
// 	}
// }

void Get_wave(void)
{
   u8 j;  
   TXDCS_1;
// 		Mp3WriteRegister(SPI_WRAMADDR,0x18,0x02);  
// 	 bands=Mp3ReadRegister(SPI_WRAM);					//获取频段数 
   Mp3WriteRegister(SPI_WRAMADDR,0x18,0x04);//启动频谱数据传输
   for(j=0;j<14;j++)  //读取14个频谱值（VS1003输出14个频段的频）
   wave[j]=Mp3ReadRegister(SPI_WRAM);
   TXDCS_0;
}
void LoadPatch(void) 
{
 u16 i;
 for(i=0;i<943;i++){Mp3WriteRegister(atab[i],(u8)(dtab[i]>>8),(u8)(dtab[i]&0x00ff));}
}

