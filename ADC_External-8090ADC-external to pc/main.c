

#include "stm32f4_discovery.h"
#include <stdio.h>
#include "usart.c"

void delay(void);
void small_delay(void);
void read_ch(uint8_t);
void config_pin(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
	
	
	/* USART2 Initialization */	
	USART_Configuration();
  delay();
	
	config_pin();
	delay();
	
	read_ch(0);
	delay();
	
	read_ch(1);
	delay();
	
	read_ch(2);
	delay();
	
	read_ch(3);
	delay();
	
	read_ch(4);
	delay();
	
	read_ch(5);
	delay();
	
	read_ch(6);
	delay();
	
	read_ch(7);
	delay();		
}


void read_ch(uint8_t ch)
{
	uint16_t adc_val;
	
	/*  Give Address  */
	switch(ch)
	{
		case 0: GPIO_ResetBits(GPIOE, GPIO_Pin_0);
						GPIO_ResetBits(GPIOE, GPIO_Pin_1);
						GPIO_ResetBits(GPIOE, GPIO_Pin_2);
						break;	
		
		case 1: GPIO_SetBits(GPIOE, GPIO_Pin_0);
						GPIO_ResetBits(GPIOE, GPIO_Pin_1);
						GPIO_ResetBits(GPIOE, GPIO_Pin_2);
						break;
		
		case 2: GPIO_ResetBits(GPIOE, GPIO_Pin_0);
						GPIO_SetBits(GPIOE, GPIO_Pin_1);
						GPIO_ResetBits(GPIOE, GPIO_Pin_2);
						break;
		
		case 3: GPIO_SetBits(GPIOE, GPIO_Pin_0);
						GPIO_SetBits(GPIOE, GPIO_Pin_1);
						GPIO_ResetBits(GPIOE, GPIO_Pin_2);
						break;
		
		case 4: GPIO_ResetBits(GPIOE, GPIO_Pin_0);
						GPIO_ResetBits(GPIOE, GPIO_Pin_1);
						GPIO_SetBits(GPIOE, GPIO_Pin_2);
						break;
						
		case 5: GPIO_SetBits(GPIOE, GPIO_Pin_0);
						GPIO_ResetBits(GPIOE, GPIO_Pin_1);
						GPIO_SetBits(GPIOE, GPIO_Pin_2);
						break;
		
		case 6: GPIO_ResetBits(GPIOE, GPIO_Pin_0);
						GPIO_SetBits(GPIOE, GPIO_Pin_1);
						GPIO_SetBits(GPIOE, GPIO_Pin_2);
						break;
		
		case 7: GPIO_SetBits(GPIOE, GPIO_Pin_0);
						GPIO_SetBits(GPIOE, GPIO_Pin_1);
						GPIO_SetBits(GPIOE, GPIO_Pin_2);
						break;
		
		default: break;
	}
	
	/* Latch the address */
	GPIO_SetBits(GPIOE, GPIO_Pin_11);
	small_delay();
	GPIO_ResetBits(GPIOE, GPIO_Pin_11);
	
	/* Start Conversion */
	GPIO_SetBits(GPIOE, GPIO_Pin_12);
	small_delay();
	GPIO_ResetBits(GPIOE, GPIO_Pin_12);
	
	small_delay();
	
	/* Wait for Conversion to complete */
	{
			delay();
			delay();
			delay();
			delay();
	}
	
	/* Enable the Output */
	GPIO_SetBits(GPIOE, GPIO_Pin_9);
	delay();
	
	 /* Read ADC Output */
	adc_val = GPIO_ReadInputData(GPIOC);
	
	 USART_SendData(USART2, (uint8_t) adc_val);
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	
	small_delay();
	GPIO_ResetBits(GPIOE, GPIO_Pin_12);
}


void config_pin(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_DeInit(GPIOC);
	
	/* GPIOC clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	/*  GPIOC Configuration  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_InitStructure.GPIO_Pin = GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_DeInit(GPIOE);
	
	/* GPIOE clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	/*  GPIOE Configuration  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_InitStructure.GPIO_Pin = GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
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
	}
}

void small_delay(void)
{
	uint16_t x;
	for(x=0;x<0xffff;x++);
	for(x=0;x<0xffff;x++);
	for(x=0;x<0xffff;x++);
	for(x=0;x<0xffff;x++); 
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
