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
#include "ws_AT45DBXX.h"
#include <string.h>
#include <stdio.h>
#include "flash.c"
#include "mmmaxx.c"

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup IOToggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

void GPIO_Configuration(void);
void Delay(__IO uint32_t nCount);
void delay(void);
void USART2_IRQHandler(void); 
void int2bin(u8 value, char buffer[]); 
 
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	const unsigned char GET_SIZE[5] = {0x56, 0x00, 0x34, 0x01, 0x00};
const unsigned char COMPRESSION_RATIO[] = {0x56, 0x00, 0x31, 0x05, 0x01, 0x01, 0x12, 0x04, 0x00};
const unsigned char RESET_CAMERA[5] = {0x00, 0x56, 0x00, 0x26, 0x00};
const unsigned char TAKE_PICTURE[5] = {0x56, 0x00, 0x36, 0x01, 0x00};
const unsigned char STOP_TAKING_PICS[5] = {0x56, 0x00, 0x36, 0x01, 0x03};
const unsigned char IMAGE_SIZE[5] = {0x56, 0x00, 0x54, 0x01, 0x11};
const unsigned char FILE_SIZE[5] = {0x56, 0x00, 0x34, 0x01, 0x00};
const unsigned char BAUD_RATE[] = {0x56, 0x00, 0x24, 0x03, 0x01, 0xAE, 0xC8};
unsigned char READ_DATA[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00,0x00};
   u16 q=0,address=0;
	u8 buf[256];
  u8 f=0,v=0;
	u16 pa=00,ba=0x0000,n=60;
	u8 buf[256],op[256];
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f2xx.c file
     */
	u8 state;
	u8 ID[4],sec[8]={0xff,0,0,45};
	//u8 image1[256],memory[264],led[4],n;
	
	u16 num,i,j;
 

	GPIO_Configuration();	
	USART_Configuration();
	//RCC_Configuration(); 
	
	
	USART_NVIC_Config();
USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	
	  /* Enable the USARTx Receive interrupt: this interrupt is generated when the
  USARTx receive data register is not empty */
	
	//printf("\r\nWELCOME TO SPI DATA LOGGING\r\n");

	AT45DBXX_Init();
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	printf("AT45DBXX had been Init!\r\n");
	
	state= Status_read();
//printf(" status register value 0x %x ",state);
	
	AT45DBXX_Read_ID(ID);
	
//printf("\r\n AT45DBXX ID is");
	/*	for(num=0;num<4;num++)
	{
		printf(" 0x%x ",ID[num]);
	}
	printf("\r\n");*/
	
		for(v=0;v<sizeof(RESET_CAMERA);v++)
{
 USART_SendData(USART2, RESET_CAMERA[v]);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  	
}



	delay();
delay();
delay();
delay();
delay();

	
for(v=0;v<sizeof(BAUD_RATE);v++)
{
 USART_SendData(USART2, BAUD_RATE[v]);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  	
}


	

USART_Configuration9600();


for(v=0;v<sizeof(IMAGE_SIZE);v++)
{
 USART_SendData(USART2, IMAGE_SIZE[v]);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  	
}

delay();

for(v=0;v<sizeof(TAKE_PICTURE);v++)
{
 USART_SendData(USART2, TAKE_PICTURE[v]);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  	
}



for(v=0;v<sizeof(FILE_SIZE);v++)
{
 USART_SendData(USART2, FILE_SIZE[v]);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  	
}


GPIO_SetBits(GPIOD , GPIO_Pin_12);
delay();
 
for(i=0;i<62;i++)
{
	q=0;
	for(v=0;v<sizeof(READ_DATA);v++)
  {
    USART_SendData(USART2, READ_DATA[v]);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  	
  }
	USART_SendData(USART2, (address>>8));
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, address);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, 0x00);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, 0x00);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, 0x00);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, 0xF8);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, 0x00);
	
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	USART_SendData(USART2, 0xFF);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

	Delay(0xffffff);
	//while(f!=1);
//delay();
//delay();
		
			MainWrite(buf, pa, ba,248, BUFFER1);
	     AT45DBXX_BUSY();
		//q=0;
	address=address+0xf8;
	pa++;
	
}	
delay();
//delay();
	for(v=0;v<sizeof(STOP_TAKING_PICS);v++)
{
 USART_SendData(USART2, STOP_TAKING_PICS[v]);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  	
}

	USART_3Configuration();
	pa=0x00;
ba=0x00;
for(i=0;i<62;i++)
{
	AT45DBXX_BUSY();
	PageRead (op,pa, ba, 248);
	
	for(j=0;j<248;j++)
	{			//printf("\r\n %0x",op[i]);
	USART_SendData(USART3, op[j]);
	  while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
  }
	pa++;
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
void Delay(__IO uint32_t nCount)
{
    while(nCount--)
  {

  }
}




void USART2_IRQHandler(void) 
{
		u8 data; 
	
	 /* Pick up the character, prepare its binary form */ 
	data = USART_ReceiveData(USART2); 
 if((q < 253) && (q>4))
	
 		{
 			buf[q-5]=data;
	 
 		}

	
	q++;	
	
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
