#ifndef MAIN_H
 #define MAIN_H

 #include "ax25.h"

 #define TRUE 1
 #define FALSE 0

 /* Define ports */
 
 /* States for the communication system */
 /* G3RUH scramble */
#define SCRAMBLE FALSE

 /* Addresses, 6 ascii charaters, pad with white spaces */
 #define SENDER "STDSAT"
#define RECEIVER "EARTH "

 /* Size of ring buffer for packets */
 /* Must be of 2^n size */
#define RB_SIZE 8
 #define TX_RB_SIZE 256
 #define RX_RB_SIZE 256

 #define USART_SIZE 258

 /* Data buffer for receiving USART data */
 typedef struct USART_BUFFER
 {
 uint8_t buffer[USART_SIZE];
 uint16_t ptr;
 }USART_BUFFER_t;

 /* Ring buffer for buffering several packets prior to sending
*/
 typedef struct TX_RING_BUFFER
 {


	 uint8_t buffer[TX_RB_SIZE * RB_SIZE];
 uint8_t count;
 uint8_t buffer_fill;
 uint8_t *head;
 uint8_t *tail;
 uint8_t *temp;
 }TX_RING_BUFFER_t;

 /* Ring buffer for buffering several received frames */
 typedef struct RX_RING_BUFFER
 {
uint8_t buffer[RX_RB_SIZE * RB_SIZE];
uint8_t count;
 uint8_t *head;
uint8_t *tail;
}RX_RING_BUFFER_t;

 /* Register for keeping frame configuration settings */
 typedef struct FRAME_CONFIGURATION
 {
 uint8_t delay;
 uint8_t preamble;
 uint8_t add_head;
 uint8_t add_tail;
 }FRAME_CONFIGURATION_t;

 /* Protocol identifiers for the CubeSTAR protocol */
 /* Can be extended to other packet types here, e.g. ACK/NACK*/
 /* Function declarations */

 void config_frame(uint8_t preamble, uint8_t head, uint8_t tail, uint8_t delay);
 void create_frame(uint8_t *data, uint8_t size);
 void send_frame(uint8_t *buf_data, uint8_t size);
 //void resend_frames();
 void bang_out(TX_BUFFER_t *buf);
 void byte_out(uint8_t byte);
 void bit_out(uint8_t bit);

 void tx_ring_buffer_init(TX_RING_BUFFER_t *tx_rb);
 uint8_t tx_ring_buffer_push(TX_RING_BUFFER_t *tx_rb, const uint8_t *tx_data);
 uint8_t tx_ring_buffer_pop(TX_RING_BUFFER_t *tx_rb, uint8_t *tx_data);
 void tx_ring_buffer_reset(TX_RING_BUFFER_t *tx_rb);
 void tx_ring_buffer_set(TX_RING_BUFFER_t *tx_rb);
 void rx_ring_buffer_init(RX_RING_BUFFER_t *rx_rb);
 uint8_t rx_ring_buffer_push(RX_RING_BUFFER_t *rx_rb, uint8_t *rx_data);
 uint8_t rx_ring_buffer_pop(RX_RING_BUFFER_t *rx_rb, uint8_t *rx_data);

//void usart_buffer_clr();
 //void from_usart();

 //void test_frame(uint8_t preamble, uint8_t head, uint8_t tail, uint8_t delay, uint8_t data, uint8_t frames_lo, uint8_t frames_hi);
 //void test_frame_command(enum commands id, uint8_t data_length);
//void test_1(uint8_t data, uint8_t frames_lo, uint8_t frames_hi);
//void set_state(uint8_t next_state);
#endif
