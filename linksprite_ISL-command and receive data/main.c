#include "stm32f4_discovery.h"
#include "mmmaxx.c"
#include <stdio.h>
#include "usart.c"
#include "image.h"

void delay(void);
uint8_t data_buf[260],size=4;
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
const unsigned char IMAGE_SIZE[5] = {0x56, 0x00, 0x54, 0x01, 0x00};
const unsigned char FILE_SIZE[5] = {0x56, 0x00, 0x34, 0x01, 0x00};
const unsigned char BAUD_RATE[] = {0x56, 0x00, 0x24, 0x03, 0x01, 0xAE, 0xC8};
unsigned char READ_DATA[] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0xFF};
//{0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0xB8, 0x00, 0xFF};

//uint16_t address = 0x0000; 


int main(void)
{
	
	//uint8_t im[] = {0,1,2,3,4,5,6,7,8,9};
	uint32_t v;
	uint16_t i = 0,x; 
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
		
	USART_Configuration();
	/*for(v=0;v<257;v++)

	data_buf[v]=v;
	/*data_buf[0]= 0x04;
	
	 data_buf[1]= 0x06;
	
	 data_buf[2]=0x02;
	 data_buf[3]=0x32;
	data_buf[4]= 0x06;
	data_buf[5]= 0x06;
	data_buf[6]= 0x06;
	data_buf[7]= 0x06;

 
  config_frame(0, 0, 0,3);//configurin frame preamble,head,tail and dlay

 send_frame( data_buf,25); //adding info to frame
*/ 


delay();
delay();

for(v=0;v<sizeof(RESET_CAMERA);v++)
{
 USART_SendData(USART2, RESET_CAMERA[v]);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  	
}

for(v=0;v<5;v++)
{
	delay();
}

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

delay();
delay();

for(v=0;v<sizeof(FILE_SIZE);v++)
{
 USART_SendData(USART2, FILE_SIZE[v]);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  	
}

delay();
delay();

//for(i=0; i<1024; i++) //1024
{
	for(v=0;v<sizeof(READ_DATA);v++)
  {
    USART_SendData(USART2, READ_DATA[v]);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
  	
  }
// 	USART_SendData(USART2, (address>>8));
// 	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
// 	USART_SendData(USART2, address);
// 	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
// 	USART_SendData(USART2, 0x00);
// 	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
// 	USART_SendData(USART2, 0x00);
// 	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
// 	USART_SendData(USART2, 0xFF);
// 	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
// 	USART_SendData(USART2, 0xFF);
// 	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
// 	USART_SendData(USART2, 0x00);
// 	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
// 	USART_SendData(USART2, 0x0A);
// 	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	
	//address=+0x40;
	
	for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);
	for(x=0;x<0xffff;x++);for(x=0;x<0xffff;x++);
}

for(v=0;v<30;v++)
{
	delay();
}

delay();
delay();

// for(v=0;v<sizeof(STOP_TAKING_PICS);v++)
// {
//  USART_SendData(USART2, STOP_TAKING_PICS[v]);
// 	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
//   	
// }

delay();
delay();

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
