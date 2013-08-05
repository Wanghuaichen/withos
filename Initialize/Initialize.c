#include "Initialize.h"
#include "ff.h"
#include "diskio.h"
#include "Hardwear.h"

TIM_TimeBaseInitTypeDef  TIM4_TimeBaseStructure;
TIM_OCInitTypeDef        TIM4_OCInitStructure;
TIM_BDTRInitTypeDef      TIM4_BDTRInitStructure;
	
extern FATFS fs; 
extern SDCardInfo SDCard_Info;

//#define RTCClockSource_LSI   /* Use the internal 32 KHz oscillator as RTC clock source */
#define RTCClockSource_LSE   /* Use the external 32.768 KHz oscillator as RTC clock source */
/*******************************************************************************
* Function Name  : fputc
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)
{
  USART1_Putchar((u8) ch);
  return ch;
}
int fgetc(FILE *f)
{
  /* Loop until received a char */
  while(!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET));
  /* Read a character from the USART and RETURN */
  return (USART_ReceiveData(USART1));
}
/*
********************************************************************************
** º¯ÊýÃû³Æ £º RCC_Configuration(void)
** º¯Êý¹¦ÄÜ £º Ê±ÖÓ³õÊ¼»¯
** Êä    Èë	£º ÎÞ
** Êä    ³ö	£º ÎÞ
** ·µ    »Ø	£º ÎÞ
********************************************************************************
*/
void RCC_Configuration(void)
{   
	ErrorStatus HSEStartUpStatus;
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus == SUCCESS)
  {
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 
    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_1);
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    /* PLLCLK = 8MHz * 10 = 80 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_10);
    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);
    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource()!= 0x08);
  }  
}

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SD_SPI_Init(void)
{
//  SPI_InitTypeDef  SPI_InitStructure; 
  GPIO_InitTypeDef GPIO_InitStructure;
  /* Enable SPI2 GPIOB clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  /* Configure SPI2 pins: SCK, MISO and MOSI  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;	//
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 ;//MISOÓ¦¸ÃÒª³õÊ¼»¯Îª´øÉÏÀ­µÄÊäÈë 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
  /* Configure PB.12 as Output push-pull, used as Flash Chip select */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  /* SPI1 configuration */ 
  SPI1_SetSpeed(SPI_SPEED_2);
}
/*******************************************************************************
* Function Name  : SPI_SetSpeed
* Description    : SPI1ÉèÖÃËÙ¶È
* Input          : u8 Speed 
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_SetSpeed(u8 Speed)
{
  SPI_InitTypeDef  SPI_InitStructure;

  SPI_Cmd(SPI1, DISABLE);            //±ØÐëÏÈ½ûÄÜ,²ÅÄÜ¸Ä±äMODE
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	if(Speed == SPI_SPEED_2){          //¸ßËÙ
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	} else if(Speed == SPI_SPEED_256){
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	}
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);
  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);  
}
void USART1_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;
//USART_ClockInitTypeDef USART_ClockInitStruct;
  /* USART1 configuration ------------------------------------------------------*/
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);
  /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
 // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);
 }
/**
  * @brief  Configures the FSMC and GPIOs to interface with the SRAM memory.
  *         This function must be called before any write/read operation
  *         on the SRAM.
  * @param  None 
  * @retval : None
  */
void FSMC_LCD_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
	
  p.FSMC_AddressSetupTime = 0x02;
  p.FSMC_AddressHoldTime = 0x00;
  p.FSMC_DataSetupTime = 0x02;
  p.FSMC_BusTurnAroundDuration = 0x00;
  p.FSMC_CLKDivision = 0x00;
  p.FSMC_DataLatency = 0x00;
  p.FSMC_AccessMode = FSMC_AccessMode_B;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;	  
  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

  /* Enable FSMC Bank1_SRAM Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);  
}
/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : Configures the RTC.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);
  /* Reset Backup Domain */
  BKP_DeInit();
  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);  
  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
#ifdef RTCClockOutput_Enable  
  /* Disable the Tamper Pin */
  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
                               functionality must be disabled */                             
  /* Enable RTC Clock Output on Tamper Pin */
  BKP_RTCCalibrationClockOutputCmd(ENABLE);
#endif 
  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Enable the RTC Second */  
  RTC_ITConfig(RTC_IT_SEC, ENABLE);
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32769); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}
 /*
********************************************************************************
** º¯ÊýÃû³Æ £º ADC1_Configuration(void)
** º¯Êý¹¦ÄÜ £º ADC1³õÊ¼»¯
** Êä    Èë	£º ÎÞ
** Êä    ³ö	£º ÎÞ
** ·µ    »Ø	£º ÎÞ
********************************************************************************
*/
void ADC1_Configuration(u8 type)
{
  ADC_InitTypeDef ADC_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
   /* Time Base configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//¶ÀÁ¢¹¤×÷Ä£Ê½
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//É¨Ãè·½Ê½
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//Á¬Ðø×ª»»
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//Íâ²¿´¥·¢½ûÖ¹
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//Êý¾ÝÓÒ¶ÔÆë
  ADC_InitStructure.ADC_NbrOfChannel = 1;//ÓÃÓÚ×ª»»µÄÍ¨µÀÊý
  ADC_Init(ADC1, &ADC_InitStructure);
	if (type ==0) {
		ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_71Cycles5);	//²ÉÑùÊ±¼äÎª71.5ÖÜÆÚ 
	}else if(type == 1) {	            //ÎÂ¶È²âÁ¿
		ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_71Cycles5);
		ADC_TempSensorVrefintCmd(ENABLE);
	}
  ADC_DMACmd(ADC1, ENABLE);	   //  Ê¹ÄÜ ADC1 DMA
  ADC_Cmd(ADC1, ENABLE);       //  Ê¹ÄÜ ADC1
  ADC_ResetCalibration(ADC1);  // ÖØÖÃÖ¸¶¨µÄADCµÄÐ£×¼¼Ä´æÆ÷

  while(ADC_GetResetCalibrationStatus(ADC1)); // Check the end of ADC1 reset calibration register
  ADC_StartCalibration(ADC1); // Start ADC1 calibaration
  while(ADC_GetCalibrationStatus(ADC1)); //	Check the end of ADC1 calibration
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);//Start ADC1 Software Conversion  
}
/******************************************************************************************************
****GPIO³õÊ¼»¯
******************************************************************************************************/
 void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_AFIO, ENABLE); 	
	
 /**************************GPIOC/AD******************************/
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//Ä£ÄâÊäÈëÒý½Å
//   GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//   GPIO_Init(GPIOA, &GPIO_InitStructure);

  /**************************GPIOA/ USART1******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         //USART1 TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    //¸´ÓÃÍÆÍìÊä³ö
  GPIO_Init(GPIOA, &GPIO_InitStructure);		    //A¶Ë¿Ú 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         //USART1 RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //¸´ÓÃ¿ªÂ©ÊäÈë
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
  /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
}
/*
********************************************************************************
** º¯ÊýÃû³Æ £º TIM1_Configuration(void)
** º¯Êý¹¦ÄÜ £º ¶¨Ê±Æ÷1³õÊ¼»¯
** Êä    Èë	£º ÎÞ
** Êä    ³ö	£º ÎÞ
** ·µ    »Ø	£º ÎÞ
********************************************************************************
*/
void TIM1_Configuration(void)
{
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE );
   /* Time Base configuration */
  TIM_DeInit(TIM1);
  TIM_TimeBaseStructure.TIM_Prescaler = 79;                   //ÉèÖÃÔ¤·ÖÆµÆ÷·ÖÆµÏµÊý71£¬¼´APB2=72M, TIM1_CLK=72/72=1MHz £¬
                                                              //ËüµÄÈ¡Öµ±ØÐëÔÚ0x0000ºÍ0xFFFFÖ®¼ä
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊýÄ£Ê½£¨´Ó0¼Çµ½ÉèÖÃÖµÒç³ö£©
  TIM_TimeBaseStructure.TIM_Period =10000;	                   //10ms¶¨Ê±,ËüµÄÈ¡Öµ±ØÐëÔÚ0x0000ºÍ0xFFFFÖ®¼ä
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;			        //ÉèÖÃÁË¶¨Ê±Æ÷Ê±ÖÓ·Ö¸î£¬
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;		      //ÉèÖÃÁËÖÜÆÚ¼ÆÊýÆ÷Öµ£¬ËüµÄÈ¡Öµ±ØÐëÔÚ0x00ºÍ0xFFÖ®¼ä¡
  TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);	            // ¸ù¾ÝTIM_TimeBaseInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î»
  TIM_ClearFlag(TIM1, TIM_FLAG_Update);      //ÇåÖÐ¶Ï£¬ÒÔÃâÒ»ÆôÓÃÖÐ¶ÏºóÁ¢¼´²úÉúÖÐ¶Ï   
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); //Ê¹ÄÜTIM1ÖÐ¶ÏÔ´ 
   
  TIM_Cmd(TIM1, ENABLE); 		             //TIM1×Ü¿ª¹Ø£º¿ªÆô
 
}
//PWM
void TIM_PWM_Init(void)
{  
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
  /* TIM4 Peripheral Configuration */ 
  TIM_DeInit(TIM4);
  /* Time Base configuration */
  TIM4_TimeBaseStructure.TIM_Prescaler = 1;	
  TIM4_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM4_TimeBaseStructure.TIM_Period = 0xffff;
  TIM4_TimeBaseStructure.TIM_ClockDivision = 0x0;
  TIM4_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
  TIM_TimeBaseInit(TIM4,&TIM4_TimeBaseStructure);
  /* Channel 1 Configuration in PWM mode */
  TIM4_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM4_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
  TIM4_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;                  
  TIM4_OCInitStructure.TIM_Pulse = 655*50; 
  TIM4_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 
  TIM4_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;         
  TIM4_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM4_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;          
  TIM_OC1Init(TIM4,&TIM4_OCInitStructure); 
  /* Automatic Output enable, Break, dead time and lock configuration*/
  TIM4_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM4_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM4_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1; 
  TIM4_BDTRInitStructure.TIM_DeadTime = 0x75;
  TIM4_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
  TIM4_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM4_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(TIM4,&TIM4_BDTRInitStructure);
  /* TIM4 counter enable */
  TIM_Cmd(TIM4,ENABLE);
  /* Main Output Enable */
  TIM_CtrlPWMOutputs(TIM4,ENABLE);
  /* NVIC configuration */
}

void NVIC_Configuration()
{  
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  /* Enable the EXTI2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  /* Enable the USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //ÏìÓ¦ÓÅÏÈ¼¶2 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
   /* Enable the TIM1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;          //¸üÐÂÊÂ¼þ 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //ÇÀÕ¼ÓÅÏÈ¼¶0 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;          //ÏìÓ¦ÓÅÏÈ¼¶2 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //ÔÊÐíÖÐ¶Ï 
  NVIC_Init(&NVIC_InitStructure);  //Ð´ÈëÉèÖÃ
}
u8 SPI1_ReadWriteByte(u8 byte)
{
  /* Send byte through the SPI2 peripheral */
  SPI1->DR=byte;
  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  /* Return the byte read from the SPI bus */
  return SPI1->DR;
}
u8 MCU_Init(void)//µ¥Æ¬»ú³õÊ¼»¯
{
	u8 retry = 0;
	DWORD fre_clust;
	FATFS *fs1;
	
	RCC_Configuration(); 
	GPIO_Configuration();
	LCD9320_GPIO_Init();
	FSMC_LCD_Init(); 
	Touch_GPIO_Init();
// 	RTC_Configuration();
	USART1_Configuration();
// 	ADC1_Configuration(1);
	VS1003_GPIO_Init();
  Mp3Reset();
	TXDCS_0;
	printf("STM32 FSMC uC/GUI Demo...\r\n");
 	printf("SD card init...\r\n");
 	for(retry = 0;retry < 5;retry ++){
 		if(disk_initialize (0)==0){
 			printf("SD card init success!\r\n");
	/***************FAT32ÎÄ¼þÏµÍ³³õÊ¼»¯****************/
			f_mount(0, &fs);
			f_getfree("", &fre_clust, &fs1);
			SDCard_Info.TotalSize = SD_GetCapacity() >> 20;
			SDCard_Info.FreeSize = (fre_clust * fs.csize) >> 11;
			SDCard_Info.ExistFlag = 1;
			printf("TotalSize: %d\r\n",SDCard_Info.TotalSize);
			printf("FreeSize: %d\r\n",SDCard_Info.FreeSize);
 			break;
    } else if(retry==4){
			SDCard_Info.ExistFlag = 0;
 		  printf("SD card init failed!\r\n");
    }
 	}
	LoadPatch();	   
	TXDCS_0;
// 	TIM1_Configuration();
//   EXTI_Configuration();  
	NVIC_Configuration();
// 	if(retry == 5) {
// 		return 0;
// 	} else {
// 		return 1;
// 	}
	return 0;
}

