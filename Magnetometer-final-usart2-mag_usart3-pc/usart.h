#ifndef _USART_H
#define _USART_H
#define Open_USART_IRQn          USART2_IRQn 

#include <stdio.h>
#include "stm32f4xx.h"
//#include "config.h"

void USART_Configuration(void);
void USART_NVIC_Config(void);
void USART_3Configuration(void);

#endif /*_USART_H*/
