#ifndef AX25_H
 #define AX25_H

 #ifdef DEBUG
 #include "debug.h"
 #endif

 #include <inttypes.h>

 #define MAX_PACKET 276 /* Maximum packet size accepted,
this includes protocol data and
 * data payload. */
 #define MIN_PACKET 21 /* Minimum packet size accepted */

 #define ADDR_L 14 /* Size of address field, either
14,21,28 */
 #define FRAME_L 400 /* Buffer for AX.25 UI frame, extra space
for bit stuffing, preamble and addidtional flags
 ,needs to be big enough to take all data. */
 #define INFO_L 256 /* Allowed size of Info field, max 256
bytes Length */

 #define TRUE 1
 #define FALSE 0

 /* Struct for building an AX.25 UI frame */
/* Stores all the data for the unencoded frame, together with the
 * precalculated FCS for the header fields. */
 typedef struct TX_FRAME_struct
 {
uint8_t flag;
 
 uint8_t address[ADDR_L];
 uint8_t control;
 uint8_t pid;
 uint8_t info[INFO_L];
 uint16_t fcs;

 uint16_t head_fcs;
 uint8_t address_count;
 uint8_t address_bytes;
 uint16_t info_bytes;
 uint8_t frame_counter;
 } TX_FRAME_t;

 /* Struct for keeping data for the decoded AX.25 frame */
 typedef struct RX_FRAME_struct
 {
 uint8_t address[ADDR_L];
 uint8_t address_count;
 uint8_t control;
 uint8_t pid;
 uint8_t info[INFO_L];
 uint8_t info_bytes;
uint16_t fcs;
 uint8_t valid;
} RX_FRAME_t;

 /* Struct for building an AX.25 UI frame */
 /* Stores the AX.25 encoded data of one frame */
 typedef struct TX_BUFFER_struct
 {
 uint8_t frame_buffer[FRAME_L];
uint8_t hi_count;
uint8_t last_bit;
uint16_t index;
uint8_t bit_counter;
uint8_t byte;
 uint32_t lfsr;
	 
uint8_t _hi_count;
 uint8_t _last_bit;
 uint16_t _index;
uint8_t _bit_counter;
 uint8_t _byte;
 uint32_t _lfsr;
 } TX_BUFFER_t;

 /* Buffer for retaining received unencoded data */
 typedef struct RX_BUFFER_struct
{
 uint8_t byte;
 uint8_t bmask;
 uint8_t flag_detected;
uint8_t data_detected;
 uint16_t data_bytes;
 uint8_t data_buffer[MAX_PACKET];
 uint8_t last_bit;
 uint32_t lfsr;
 uint8_t hi_count;
 uint8_t flag_count;
}RX_BUFFER_t;

/* Function declarations */
 uint16_t calcFCS(uint8_t c, uint16_t fcs);

/* Tx specific function declarations */
 void init_tx_frame(TX_FRAME_t *frame);
 void _init_tx_frame(TX_FRAME_t *frame);
 void reset_tx_frame(TX_FRAME_t *frame);
void init_tx_buffer(TX_BUFFER_t *buf);
void init_tx_frame(TX_FRAME_t *frame);
void init_tx_buffer(TX_BUFFER_t *buf);
void set_address(TX_FRAME_t *frame, char *destination, uint8_t ssid1, char *source, uint8_t ssid2, char *rpt1, uint8_t ssid3 , char *rpt2, uint8_t ssid4);
 void set_info(TX_FRAME_t *frame, uint8_t *info, uint16_t sz);
void set_infoFCS(TX_FRAME_t *frame);
 void set_head_FCS(TX_FRAME_t *frame);
void byte2buf(uint8_t byte, uint8_t bitstuff, TX_BUFFER_t *buf, uint8_t scramble);
 void buffer_header(TX_FRAME_t *frame, TX_BUFFER_t *buf, uint8_t scramble);
 void buffer_frame(TX_FRAME_t *frame, TX_BUFFER_t *buf, uint8_t scramble);
 void clear_tx_buffer(TX_BUFFER_t *buf);
void buffer_byte(uint8_t byte, TX_BUFFER_t *buf, uint8_t scramble);

 /* Rx specific function declarations */
void init_rx_frame(RX_FRAME_t *frame);
void init_rx_buffer(RX_BUFFER_t *buf);
void clear_rx_buffer(RX_BUFFER_t *buf);
void add_bit(uint8_t bit, RX_FRAME_t *frame, RX_BUFFER_t *buf, uint8_t scramble);
uint8_t decode_frame(RX_FRAME_t *rx_frame, RX_BUFFER_t *rx_buf);
uint8_t verify_frame(RX_BUFFER_t *rx_buf);
#endif
