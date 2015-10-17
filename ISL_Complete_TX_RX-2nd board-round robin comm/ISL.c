/******************************************************************
 C file for ISL module (Nano Series 900MHz/2.4GHz Spread Spectrum OEM Module)
 Default Baud rate of the module is 9600
 Author : Nakul Rao I
 Date : 16/6/2013
 Written for STM32F4 Discovery
 Makes use of UART2
 ******************************************************************/
 
#include "ISL.h"
#include "usart.h"
 
void send_AT(void)
{
	USART_SendData(USART2, (uint8_t) 'A');

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
		
	USART_SendData(USART2, (uint8_t) 'T');

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}

void send_ENTER(void)
{
	USART_SendData(USART2, (uint8_t) 13);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}

void send_S(void)
{
	USART_SendData(USART2, (uint8_t) 'S');

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}

void send_0(void)
{
	USART_SendData(USART2, (uint8_t) '0');

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}

void send_1(void)
{
	USART_SendData(USART2, (uint8_t) '1');

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}

void send_eq(void)
{
	USART_SendData(USART2, (uint8_t) '=');

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}

void send(char c)
{
	USART_SendData(USART2, (uint8_t)c);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}
	

void command_mode(void)
{
	delay();
	delay();
	send('+');
	send('+');
	send('+');
	delay();
	delay();
}

void data_mode(void)
{
	send_AT();
	send('A');
	send_ENTER();
}

/****************************************************************************
	All the fillowing functions are to be used  only in Command Mode
*****************************************************************************/

/*  Set Module as Transmitter  */
void set_Tx(void)
{
	send_AT();
	send('&');
	send('F');
	send('6');
	send_ENTER();
	delay();
	send_AT();
	send('&');
	send('W');
	send('A');
	send_ENTER();
}

/*  Set Module as Receiver  */
void set_Rx(void)
{
	send_AT();
	send('&');
	send('F');
	send('7');
	send_ENTER();
	delay();
	send_AT();
	send('&');
	send('W');
	send('A');
	send_ENTER();
}

void read_all_reg(void)
{
	char rx_char;
	
	send_AT();
	send('&');
	send('V');
	send_ENTER();
	
	while ((USART_GetFlagStatus(USART2, USART_FLAG_RXNE)== RESET)); 
		rx_char = USART_ReceiveData(USART2);
		
		USART_SendData(USART3, (uint8_t)rx_char);
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		{}
}

void read_op_power(void)
{
	char rx_char;
	
	send_AT();
	send_S();
	send_1();
	send_0();
	send('8');
	send('?');
	send_ENTER();
	
	while ((USART_GetFlagStatus(USART2, USART_FLAG_RXNE)== RESET)); 
		rx_char = USART_ReceiveData(USART2);
		
		USART_SendData(USART3, (uint8_t)rx_char);
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		{}
}

void read_baud_rate(void)
{
	char rx_char;
	
	send_AT();
	send_S();
	send_1();
	send_0();
	send('2');
	send('?');
	send_ENTER();
	
	while ((USART_GetFlagStatus(USART2, USART_FLAG_RXNE)== RESET)); 
		rx_char = USART_ReceiveData(USART2);
		
		USART_SendData(USART3, (uint8_t)rx_char);
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		{}
}
	
	
/************************* EOF ******************************/	

