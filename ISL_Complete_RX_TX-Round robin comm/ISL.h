/******************************************************************
 Header file for ISL module (Nano Series 900MHz/2.4GHz Spread Spectrum OEM Module)
 Default Baud rate of the module is 9600
 Author : Nakul Rao I
 Date : 16/6/2013
 Written for STM32F4 Discovery
 Makes use of UART2
 ******************************************************************/
 
 #include "stdint.h"
 
 /* Function Prototypes */
 void send_AT(void);
 void send_ENTER(void);
 void send_S(void);
 void send_0(void);
 void send_1(void);
 void send_eq(void);
 void send(char);
 void command_mode(void);
 void data_mode(void);
 void set_Tx(void);
 void set_Rx(void);
 void read_all_reg(void);
 void read_op_power(void);
 void read_baud_rate(void);
 
 /********************************* EOF ***************************/
