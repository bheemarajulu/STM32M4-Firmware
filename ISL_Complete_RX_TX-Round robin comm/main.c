#include "stm32f4_discovery.h"
#include <stdio.h>
#include "usart.c"
#include "ISL.h"

void delay(void);
/**
  * @brief  Main program
  * @param  None
  * @retval None
   */
int main(void)
{
	char rx_char = 0;
	/*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
		
	USART_Configuration();
	USART_3Configuration();
	delay();
	send(' ');
	
	command_mode();
	
	/* set as Rx */
	set_Rx();
	delay();
	
	data_mode();
		
	while(rx_char!='X')
	{
		while ((USART_GetFlagStatus(USART2, USART_FLAG_RXNE)== RESET)); 
		rx_char = USART_ReceiveData(USART2);
		
		USART_SendData(USART3, (uint8_t)rx_char);
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		{}
	}
	
	command_mode();
	
	/* set as Tx */
	set_Tx();
	delay();
	 
	data_mode();
	delay();
	
	send('A');
	send('B');
	send('C');
	send('D');
	send('E');
	send('X');
	
	delay();
		
	while(1){}	
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
