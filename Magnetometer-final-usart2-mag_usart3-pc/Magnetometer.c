/**************************************************************************
	C file for Magnetometer (HMR2300)
	Date : 18/6/2013
	Author : Nakul Rao I
***************************************************************************/

#include "usart.h"

/*
	Send a single character via USART2
*/
void send(char c)
{
	USART_SendData(USART2, (uint8_t)c);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}

void send_ENTER(void)
{
	USART_SendData(USART2, 13);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}


/*
	Enable Write to magnetometer
*/
void write_EN(void)
{
	send('*');
	send('9');
	send('9');
	send('w');
	send('e');
}

/*
	Output Readings in BCD ASCII Format (Default)
	Call write_EN() before calling this function
*/
void ASCII_On(void)
{
	send('*');
	send('9');
	send('9');
	send('a');
}

/*
	Output Readings in Signed 16-bit Bianary Format
	Call write_EN() before calling this function
*/
void Binary_On(void)
{
	send('*');
	send('9');
	send('9');
	send('b');
}

/*
	Polled – Output a Single Sample (Default)
	DO NOT call write_EN() before calling this function
*/
void single_op(void)
{
	char i, rx_char;
	send('*');
	send('9');
	send('9');
	send('p');
	
	for(i=0; i<7; i++)
	{
		while ((USART_GetFlagStatus(USART2, USART_FLAG_RXNE)== RESET)); 
		rx_char = USART_ReceiveData(USART2);
		
		USART_SendData(USART3, (uint8_t)rx_char);
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		{}
	}
}

/*
	Continuous – Output Readings at Sample Rate
	DO NOT call write_EN() before calling this function
*/
void continuous_op(void)
{
	send('*');
	send('9');
	send('9');
	send('c');
}

/*
	Stops Continuous Readings	
	DO NOT call write_EN() before calling this function
*/
void stop_con_op(void)
{
	USART_SendData(USART2, 27);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}

/*
	Set BAUD Rate to 9600 
	Call write_EN() before calling this function
*/
void set_baud_9600(void)
{
	send('*');
	send('9');
	send('9');
	send('!');
	send('b');
	send('r');
	send('=');
	send('s');
}

/*	
	Set BAUD Rate to 19200 
	Call write_EN() before calling this function
*/
void set_baud_19200(void)
{
	send('*');
	send('9');
	send('9');
	send('!');
	send('b');
	send('r');
	send('=');
	send('f');
}

/*
	Change All Command Parameter Settings to Factory Default Values	
	Call write_EN() before calling this function
*/
void set_factory_default(void)
{
	send('*');
	send('9');
	send('9');
	send('d');
}

/*
	This writes all parameter settings to EEPROM. These values will be automatically restored upon power-up. 
	Call write_EN() before calling this function
*/
void store_parameters(void)
{
	send('*');
	send('9');
	send('9');
	send('s');
	send('p');
}

/*
	Change All Command Parameter Settings to the Last User Stored Values in the EEPROM
	Call write_EN() before calling this function
*/
void restore_settings(void)
{
	send('*');
	send('9');
	send('9');
	send('r');
	send('s');
	send('t');
}

/********************************************** EOF ***********************************************************/

