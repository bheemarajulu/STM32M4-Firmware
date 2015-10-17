#include "stm32f4_discovery.h"
#include <inttypes.h>
 #include <string.h>
 #include <stdio.h>
 #include "mainax.h"
 #include "ax25.c"
 #include "usart.c"
 
 void USART2_IRQHandler(void); 

 /* AX.25 UI frame info and buffers */
 RX_BUFFER_t rx_buffer;
 RX_FRAME_t rx_frame;
 FRAME_CONFIGURATION_t frame_configuration;
 
 
 int main(void)
{
	/*	!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */
  	
	init_rx_buffer(&rx_buffer);
 init_rx_frame(&rx_frame);
	
	USART_Configuration();
		USART_NVIC_Config();
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	while(1);
	

 	return 0;
}
 

void USART2_IRQHandler(void) 
{ uint8_t data,inbit,j; 
	 /* Pick up the character, prepare its binary form */ 
	data = USART_ReceiveData(USART2); 
	for(j=0;j<8;j++)
		{
			
			inbit=data & 0x01;
			add_bit(inbit, &rx_frame, &rx_buffer, SCRAMBLE); 
			data >>=1;
		}

} 
