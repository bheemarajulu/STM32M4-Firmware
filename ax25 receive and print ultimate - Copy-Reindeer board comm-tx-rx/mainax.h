#ifndef MAIN_H
 #define MAIN_H

 #include "ax25.h"
 #ifdef DEBUG
 #include "debug.h"
 #endif

 #define TRUE 1
 #define FALSE 0

 /* Define ports */
 #define CLK_PORT PORTE
 #define DATA_PORT PORTE
 #define CLK_PIN PIN2_bm
 #define DATA_PIN PIN0_bm
#define USART_PORT PORTC
 #define USART USARTC0


 /* States for the communication system */
enum states
 {
 CW_STATE,
RX_STATE,
TX_STATE,
 TX_TEST_STATE
 };

 /* G3RUH scramble */
#define SCRAMBLE FALSE

 /* Addresses, 6 ascii charaters, pad with white spaces */
 #define SENDER "CBSTAR"
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
 /* Can be extended to other packet types here, e.g. ACK/NACK
*/
 enum commands
 {
 CMD = 0x10, // command frames 108
 HK = 0x20, // housekeeping frames
 TEST = 0x30, // test frames
 PAYLOAD = 0x40, // payload data frames
 RAW = 0x50 // raw ascii frames
 };

 /* Function declarations */

 void add_byte(RX_BUFFER_t *buf);
 void handle_rx_buffer(RX_RING_BUFFER_t *rx_rb);
 void handle_data(RX_BUFFER_t *buf, RX_FRAME_t *frame);
 void config_frame(uint8_t preamble, uint8_t head, uint8_t tail, uint8_t delay);
 void create_frame(enum commands id, uint8_t *data, uint8_t size);
 
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
 #endif
 