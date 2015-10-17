/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "config.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "ws_spi.c"


/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup IOToggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

void GPIO_Configuration(void);
void Delay(__IO uint32_t nCount);
void USART2_IRQHandler(void); 
 
 
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void adf_7020config(void); 
void write_to_adf7020(unsigned long value);
void transmit_mode(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	void delay(void);
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
/*	
const unsigned char GET_SIZE[5] = {0x56, 0x00, 0x34, 0x01, 0x00};
const unsigned char COMPRESSION_RATIO[] = {0x56, 0x00, 0x31, 0x05, 0x01, 0x01, 0x12, 0x04, 0x00};
const unsigned char RESET_CAMERA[5] = {0x00, 0x56, 0x00, 0x26, 0x00};
const unsigned char TAKE_PICTURE[5] = {0x56, 0x00, 0x36, 0x01, 0x00};
const unsigned char STOP_TAKING_PICS[5] = {0x56, 0x00, 0x36, 0x01, 0x03};
//const unsigned char IMAGE_SIZE[5] = {0x56, 0x00, 0x54, 0x01, 0x11};
const unsigned char FILE_SIZE[5] = {0x56, 0x00, 0x34, 0x01, 0x00};
const unsigned char BAUD_RATE[] = {0x56, 0x00, 0x24, 0x03, 0x01, 0xAE, 0xC8};
unsigned char READ_DATA[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00,0x00};
   u16 q=0,address=0;
	u8 buf[256];
  u8 f=0;
	static u16 pa=0x0000,ba=0x0000;*/
	
	u32 reg_val=0x00;	


int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f2xx.c file
     */
	
	GPIO_Configuration();	
	USART_Configuration();
		USART_NVIC_Config();
USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	//RCC_Configuration(); 
	
	
	

	SPI_Configuration();
	SPI2_Configuration();

	//printf("SPI is ready!\r\n");

 while(1)
 {
	adf_7020config();			 //configure the adfxxxx

	transmit_mode();
	 delay();
	 SPI2_Send_byte(0x5055);

 }
	while (1)
	{
		
		GPIO_SetBits(GPIOD , GPIO_Pin_12);
		Delay(0x5fffff);
		GPIO_SetBits(GPIOD , GPIO_Pin_13);
		Delay(0x5fffff);
		GPIO_SetBits(GPIOD , GPIO_Pin_14);
		Delay(0x5fffff);
		GPIO_SetBits(GPIOD , GPIO_Pin_15);
		Delay(0x5fffff);


		GPIO_ResetBits(GPIOD , GPIO_Pin_12);
		Delay(0x5fffff);
		GPIO_ResetBits(GPIOD , GPIO_Pin_13);
		Delay(0x5fffff);
		GPIO_ResetBits(GPIOD , GPIO_Pin_14);
		Delay(0x5fffff);
		GPIO_ResetBits(GPIOD , GPIO_Pin_15);
		Delay(0x5fffff);
	}
}
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*--------------------------------------------------------------------------------
	
	LED GPIO Config
	
	--------------------------------------------------------------------------------*/
	/* GPIOC Periph clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}



/**
  * @param  nCount:specifies the Delay time length.
  * @retval None
  * @brief  Delay Function.
  */
void Delay(__IO uint32_t nCount)
{
    while(nCount--)
  {

  } 
}
void USART2_IRQHandler(void) 
{
	/*u8 data; 
	
	 /* Pick up the character, prepare its binary form 
	data = USART_ReceiveData(USART2); 
 	if((q < 253) && (q>4))
 		{
 			buf[q-5]=data;
	 
 		}
else
{ f=1;}
	
	q++;	*/
} 

void delay(void)
{
	uint16_t i,x;
	for(i=0;i<0x20;i++)
{
for(x=0;x<0xffff;x++);
for(x=0;x<0xffff;x++);
for(x=0;x<0xffff;x++);
for(x=0;x<0xffff;x++); 
for(x=0;x<0xffff;x++); 
for(x=0;x<0xffff;x++);
for(x=0;x<0xffff;x++); 
for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);
for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);
for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);
for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);
for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);
for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);
}
}

//-------------------------------------------------------//
// void adf_7020config(void)

// Description :
// This Function is used to configure the adfxxxx register  
//-------------------------------------------------------//
void adf_7020config(void)   //19200
{ 
 
   reg_val=0x2DD801C6;
   write_to_adf7020(reg_val);
   Delay(100);
   reg_val=0x7939be80;		// register 0 - N register
   write_to_adf7020(reg_val);
   reg_val=0x00029011;		// register 1 - oscillator/filter register
   write_to_adf7020(reg_val);
   reg_val=0xCD097E52;		// register 2 - transmit modulation register(ASK/OOK/FSK/GFSK/GOOK mode)
   write_to_adf7020(reg_val);
   reg_val=0x006F1253;		// register 3 - receiver clock register
   write_to_adf7020(reg_val);
   reg_val=0x01000254;		// register 4 - demodulator set-up register
   write_to_adf7020(reg_val);
   reg_val=0x72472635;		// register 5 - sync byte register
   write_to_adf7020(reg_val);
   
}
// void transmit_mode(void)

// Description :
// This Function is used to write the 32 bit register value
// into the adfxxxx for configure the transmit mode  
//-------------------------------------------------------//
void transmit_mode(void)
{  
    reg_val=0x7139e390;
    write_to_adf7020(reg_val);
}
//-------------------------------------------------------//
// void write_to_adf7020(unsigned long value)

// Description :
// This Function is used to write the data's into adfxxxx
// module  
//-------------------------------------------------------//
void write_to_adf7020(unsigned long value)
{
SPI_Send_byte(value>>16);
	SPI_Send_byte(value);
}



#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {

  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
