#include <inttypes.h>
 #include <ctype.h>
 #include <string.h>
 #include <stdio.h>
 #include "mmmaxx.h"
 #include "ax25.c"

 /* AX.25 UI frame info and buffers */
 TX_FRAME_t tx_frame;
 TX_BUFFER_t tx_buffer;
 RX_BUFFER_t rx_buffer;
 RX_FRAME_t rx_frame;
 FRAME_CONFIGURATION_t frame_configuration;

 TX_RING_BUFFER_t tx_packet_buffer;
 RX_RING_BUFFER_t rx_packet_buffer;
 USART_BUFFER_t usartrx_buffer;


 /* Global declarations */
 volatile uint8_t current_state;
 volatile uint8_t inbit;
 volatile uint8_t usart_data;
 volatile uint8_t frame_counting;
 volatile uint8_t frames_to_resend;

void _delay_ms(uint32_t nCount);
 /* Replace with functions for SPI bus */
/*create HOUSEKEEPING frame */
 /*if(usartrx_buffer.ptr > 0)
 {
 if(usartrx_buffer.ptr == ((uint16_t) (usartrx_buffer.buffer[1] + 2)))
 {
 create_frame(HK, (usartrx_buffer.buffer + 2),usartrx_buffer.buffer[1]);
 usart_buffer_clr();
 set_state(TX_STATE);
 
 }
if(usartrx_buffer.ptr == 5)
 {
 config_frame(usartrx_buffer.buffer[1], usartrx_buffer.buffer[2], usartrx_buffer.buffer[3], usartrx_buffer.buffer[4]);
 usart_buffer_clr();
 }
 break;*/

 /* RX used to initiate test functions and send frames */
 /* TX used to send debug messages and received KISS data */

 /* Configures TX standard parameters for all frames */
 void config_frame(uint8_t preamble, uint8_t head, uint8_t tail,uint8_t delay)
 {
 uint8_t i;

 init_tx_buffer(&tx_buffer);
 init_tx_frame(&tx_frame);

 frame_configuration.delay = delay; /* Delay between frames */
frame_configuration.preamble = preamble; /* Bytes of preamble */
frame_configuration.add_head = head; /* Number of additional head flags */
 frame_configuration.add_tail = tail; /* Number of
additional tail flags */


 /* Buffer preamble and header data */
 for(i=0; i<frame_configuration.preamble; i++)
 buffer_byte(0x00, &tx_buffer, FALSE);

 for(i=0; i<frame_configuration.add_head; i++)
 buffer_byte(0x7E, &tx_buffer, SCRAMBLE);

set_address(&tx_frame,RECEIVER, 0x00,SENDER, 0x00, NULL, 0x00, NULL, 0x00);
buffer_header(&tx_frame, &tx_buffer, SCRAMBLE);
}


 /* Reads data to be sent from TX ring buffer. Encodes data, and
sends it. */
 void send_frame(uint8_t *buf_data,uint8_t size)
 {
	 uint8_t i;
buf_data[size]='\0';
 reset_tx_frame(&tx_frame);
 clear_tx_buffer(&tx_buffer);

 set_info(&tx_frame, buf_data, size);

 buffer_frame(&tx_frame, &tx_buffer, SCRAMBLE);

 for(i=0; i<frame_configuration.add_tail; i++)
 buffer_byte(0x7E, &tx_buffer, SCRAMBLE);

 bang_out(&tx_buffer);
 _delay_ms(frame_configuration.delay);
}

 /* Clock out buffered frame in serial synchronus transmit mode
*/
 void bang_out(TX_BUFFER_t *buf)
 {
 uint16_t i, j;

 uint8_t bmask, bit;

 for(i=0; i<buf->index; i++)
 {
 byte_out(buf->frame_buffer[i]);
 }

 bmask = 0x01;
 /* Clock out only valid bits in the last byte */
 for(j=0; j<buf->bit_counter; j++)
 {
 bit = ((buf->frame_buffer[i]) & bmask) >> j;
 bit_out(bit);
 bmask <<= 1;
 }

 /* Set ouput to low */
 bit_out(0x00);
 }

 /* Clock out buffered frame in serial synchronus transmit mode
, byte by byte */
 void byte_out(uint8_t byte)
 {
 uint8_t bit;
 uint8_t i;
 uint8_t bmask;

 bmask = 0x01;

 for(i=0; i< 8; i++)
 {
 bit = (byte & bmask) >> i;
 bit_out(bit);
 bmask <<= 1;
 }
 }

 /* Clock out buffered frame in serial synchronus transmit mode
, bit by bit*/
 void bit_out(uint8_t bit)
 {
 /* Wait for signal from transceiver */
// while(!(CLK_PORT.IN & CLK_PIN));
 //DATA_PORT.OUT = bit;
 //while(CLK_PORT.IN & CLK_PI
 }

/** Ring buffer functions **/
 /* TX ring buffer used to store packets prior to AX.25
encoding and sending */
 void tx_ring_buffer_init(TX_RING_BUFFER_t *tx_rb)
 {
 tx_rb->count = 0;
 tx_rb->buffer_fill = 0;
 tx_rb->head = &tx_rb->buffer[0];
 tx_rb->tail = &tx_rb->buffer[0];
 }

 /* Push tx_data in to tx_rb buffer */
 uint8_t tx_ring_buffer_push(TX_RING_BUFFER_t *tx_rb, const uint8_t *tx_data)
 {
 uint16_t i;

 if(tx_rb->count == RB_SIZE)
 return 1;

 for(i=0; i<TX_RB_SIZE; i++)
 *(tx_rb->head)++ = tx_data[i];

 tx_rb->count++;
 if(tx_rb->buffer_fill < RB_SIZE)
 tx_rb->buffer_fill++;

 if(tx_rb->head == &tx_rb->buffer[TX_RB_SIZE * RB_SIZE])
 tx_rb->head = &tx_rb->buffer[0];

 return 0;
 }

 /* Pop tx_data from tx_rb buffer */
 uint8_t tx_ring_buffer_pop(TX_RING_BUFFER_t *tx_rb, uint8_t *tx_data)

{
 uint16_t i;

 // if(tx_rb->count == 0)
 // return 1;

 for(i=0; i<TX_RB_SIZE; i++)
 tx_data[i] = *(tx_rb->tail)++;

 tx_rb->count--;

 if(tx_rb->tail == &tx_rb->buffer[TX_RB_SIZE * RB_SIZE])
 tx_rb->tail = &tx_rb->buffer[0];

 return 0;
 }

 /* Moves tail pointer to the oldest data, for resending the
hole buffer */
 void tx_ring_buffer_reset(TX_RING_BUFFER_t *tx_rb)
 {
 tx_rb->temp = tx_rb->tail;
 tx_rb->count += frames_to_resend;

 if(tx_rb->buffer_fill < RB_SIZE)
 tx_rb->tail = &tx_rb->buffer[0];
 else
 tx_rb->tail = tx_rb->head + TX_RB_SIZE;
 }

 /* Set tail pointer to the correct place */
 void tx_ring_buffer_set(TX_RING_BUFFER_t *tx_rb)
 {
 tx_rb->tail = tx_rb->temp;
 }
 
 
 void _delay_ms(uint32_t nCount)
{
   uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}
 


 /* Main function */
/*int main()
 {
 uint8_t data_buf[255];
	 
	 config_frame(1, 2, 2,3);

 create_frame( data_buf, 5);
 while(1)
 {
 send_frame();

}
 return 0;
}*/
