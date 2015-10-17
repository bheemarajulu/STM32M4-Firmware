#include "stm32f4_discovery.h"
#include "mmmaxx.c"
#include <stdio.h>
#include "usart.c"

void delay(void);
uint8_t data_buf[260],size=4;
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	uint16_t v;
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
		
	USART_Configuration();
	for(v=0;v<257;v++)

	data_buf[v]=v;
	/*data_buf[0]= 0x04;
	
	 data_buf[1]= 0x06;
	
	 data_buf[2]=0x02;
	 data_buf[3]=0x32;
	data_buf[4]= 0x06;
	data_buf[5]= 0x06;
	data_buf[6]= 0x06;
	data_buf[7]= 0x06;

 */
  config_frame(0, 0, 0,3);//configurin frame preamble,head,tail and dlay

 send_frame( data_buf,25); //adding info to frame
 delay();

for(v=0;v<=tx_buffer.index;v++)
{
 USART_SendData(USART2, tx_buffer.frame_buffer[v]);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	if((v%11==0)&&(v!=0))
		delay();
}
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
