#include "ax25.h"

 #ifdef DEBUG
 #include "debug.h"
 #endif

 /* Bit wise calculate the FCS */
 uint16_t calcFCS(uint8_t byte, uint16_t fcs)
 {
 uint8_t bit;

 for(bit=0; bit<8; bit++)
 {
 fcs ^= (byte & 0x01);
 fcs = (fcs & 0x01) ? (fcs >> 1) ^ 0x8408 : (fcs >> 1);
 byte = byte >> 1;
 }

 return fcs;
 }

 /* *******************************************************
 * Functions for Tx.
 * *******************************************************/

 /* Initialize one AX.25 UI frame, resets all values */
 void init_tx_frame(TX_FRAME_t *frame)
 {
 reset_tx_frame(frame);
 _init_tx_frame(frame);
 }

 /* Resets only the frame counter of an AX.25 UI frame */
 void _init_tx_frame(TX_FRAME_t *frame)
 {
	 uint16_t i; 
 frame->flag = 0x7E;
 frame->control = 0x03;
 frame->pid = 0xF0;
 frame->head_fcs = 0xFFFF;



 for(i=0; i<ADDR_L; i++)
 {
 frame->address[i] = 0;
 }
 frame->address_bytes = 0;
 frame->address_count = 0;
 frame->frame_counter = 0;
 }


 /* Resets all values, but frame counter and address in an AX.25 frame */
void reset_tx_frame(TX_FRAME_t *frame)
 {
 uint16_t i;

 frame->fcs = 0xFFFF;
 for(i=0; i<INFO_L; i++)
 {
 frame->info[i] = 0;
	 
	 
}

 frame->info_bytes = 0;
 }

 /* Initialize one buffer for AX.25 UI frame(s) */
 void init_tx_buffer(TX_BUFFER_t *buf)
 {
 uint16_t i;
 for(i=0; i<FRAME_L; i++)
 {
 buf->frame_buffer[i] = 0x00;
 }

buf->hi_count = 0;
 buf->last_bit = 0;
 buf->index = 0;
 buf->bit_counter = 0;
 buf->byte = 0x00;
 buf->lfsr = 0L;
 buf->_hi_count = 0;
 buf->_last_bit = 0;
 buf->_index = 0;
 buf->_bit_counter = 0;
 buf->_byte = 0x00;
 buf->_lfsr = 0L;
 }

 /* Set the send/recv address and the optional repeateraddresses */
 void set_address(TX_FRAME_t *frame, char *destination, uint8_t ssid1, char *source, uint8_t ssid2, char *rpt1, uint8_t ssid3 , char *rpt2, uint8_t ssid4)
 {
 uint8_t i, j;

 uint8_t source_addr[7];
 uint8_t dest_addr[7];
 uint8_t rpt1_addr[7];
 uint8_t rpt2_addr[7];

 /* Shift addresses one bit source the left. AX.25 standard*/
 for(i=0; i<6; i++)
 {
 dest_addr[i] = destination[i] << 1;
 source_addr[i] = source[i] << 1;
 }

 dest_addr[6] = (ssid1 << 1) | 0x60;
 source_addr[6] = (ssid2 << 1) | 0x60;
 frame->address_bytes = 14;

 if(rpt1)
 {
frame->address_count++;
 for(i=0; i<6; i++)
 {
 rpt1_addr[i] = rpt1[i] << 1;
frame->address_bytes++;
 }
 rpt1_addr[6] = (ssid3 << 1) | 0x60;
 frame->address_bytes++;
 }

 if(rpt2)
 {
 frame->address_count++;
 for(i=0; i<6; i++)
 {
 rpt2_addr[i] = rpt2[i] << 1;
 frame->address_bytes++;
 }
 rpt2_addr[6] = (ssid3 << 1) | 0x61;
 frame->address_bytes++;
 }

/* Set correct address extension bit */
 if(!rpt1)
 source_addr[6] |= 0x01;
 else if((rpt1) && (!rpt2))
 rpt1_addr[6] |= 0x01;

/* Copy addresses to frame */
 j = 0;
 for(i=0; i<7; i++)
 {
 frame->address[j] = dest_addr[i];
j++;

 }

 for(i=0; i<7; i++)
{
 frame->address[j] = source_addr[i];
 j++;


 }

 if(ADDR_L > 14)
 {
 if(rpt1)
 {
 for(i=0; i<7; i++)
 {
 frame->address[j] = rpt1_addr[i];
 j++;

 }
 }

 }

 if(ADDR_L > 21)
 {
 if(rpt2)
 {
 for(i=0; i<7; i++)
 {
 frame->address[j] = rpt2_addr[i];
 j++;

 }
 }

 }

 /* Pre-calculate the FCS for the header fields */
 set_head_FCS(frame);
 }


 /* Set the info field and calculate FCS for the frame */
 void set_info(TX_FRAME_t *frame, uint8_t *info, uint16_t sz)
 {
 uint16_t i;
 for(i=0; i<=sz; i++)
 frame->info[i] = info[i];
 frame->info_bytes = sz+1;

 set_infoFCS(frame);
}

 /* Calculates the FCS for the info field, FCS for header fields
 * must have been precalculated. I.e. address must have been set prior
 * to setting the info. */
 void set_infoFCS(TX_FRAME_t *frame)
 {
 uint16_t sum, i;

uint8_t c;

 sum = frame->head_fcs;

 for(i=0; i<frame->info_bytes; i++)
 {
 c = frame->info[i];
 sum = calcFCS(c, sum);
 }

 frame->fcs = ~sum;
 }

 /* Precalculate FCS for address, controll and pid, called from setAddress() */
 void set_head_FCS(TX_FRAME_t *frame)
 {
 int i;
 //unsigned int sum;
 uint16_t sum;

for(i=0; i<frame->address_bytes; i++)
 {
 sum = calcFCS(frame->address[i], frame->head_fcs);

 frame->head_fcs = sum;
 }

 sum = calcFCS(frame->control, frame->head_fcs);

 frame->head_fcs = sum;
 sum = calcFCS(frame->pid, frame->head_fcs);

 frame->head_fcs = sum;
 }


 /* Inspect byte bit for bit for bitstuffing, NRZ-I encoding and G3RUH scrambling if necessary */
 /* Returns 1 if buffer is full */
 void byte2buf(uint8_t byte, uint8_t bitstuff, TX_BUFFER_t *buf, uint8_t scramble)
 {
 uint16_t i;
 uint8_t bmask, bit;
// uint8_t hi_count;
 uint8_t outbit;
 uint8_t lfsr_hi, lfsr_lo, lfsr_bit, scr_bit;
	 bmask = 0x01;

 /* Bitwise inspect the given byte */
 for(i=0; i<8; i++)
 {
 bit = (bmask & byte);
 bmask <<= 1;
 bit >>= i;

/* Perform NRZ-I encoding */
 /* Check if this bit is 1 */
 if(bit)
 {
 //hi_count = buf->hi_count;
 outbit = buf->last_bit; /* Next bit is same as previous */
 buf->last_bit = outbit;

 /* Perform scrambling according to G3RUH standard */
 if(scramble)
 {
 lfsr_hi = (buf->lfsr & 0x00000001L);
 lfsr_lo = ((buf->lfsr >> 5) & 0x01L);
 lfsr_bit = lfsr_hi ^ lfsr_lo;

 scr_bit = outbit ^ lfsr_bit;
 buf->byte |= scr_bit << buf->bit_counter;
 buf->lfsr >>= 1;
 if(scr_bit) buf->lfsr |= 0x00010000L;
 }
 else
 buf->byte |= (outbit << buf->bit_counter);

 buf->bit_counter++;
 buf->frame_buffer[buf->index] = buf->byte;
 buf->hi_count++;

 /* Check if we have one byte to add to buffer */
 if(buf->bit_counter == 8)
{
 buf->index++;
 buf->byte = 0x00;
 buf->bit_counter = 0;
 }
 /* Check for bit stuffing */
 if(buf->hi_count == 5 && bitstuff == TRUE)
 {
 /* Perform NRZ-I encoding */
 outbit = (~(buf->last_bit) & 0x01); /* Next bit is inverse of previous */
 buf->last_bit = outbit;

 /* Perform scrambling according to G3RUH standard */
 if(scramble)
 {
 lfsr_hi = (buf->lfsr & 0x00000001L);
 lfsr_lo = ((buf->lfsr >> 5) & 0x01L);
 lfsr_bit = lfsr_hi ^ lfsr_lo;

 scr_bit = outbit ^ lfsr_bit;
 buf->byte |= (scr_bit << buf->bit_counter);
 buf->lfsr >>= 1;
 if(scr_bit) buf->lfsr |= 0x00010000L;
 }
 else
 buf->byte |= (outbit << buf->bit_counter);

 buf->bit_counter++;
 buf->frame_buffer[buf->index] = buf->byte;
 buf->hi_count = 0;

 /* Check if we have one byte to add to buffer */
 if(buf->bit_counter == 8)
 {
 buf->index++;
 buf->byte = 0x00;
 buf->bit_counter = 0;
 }
 }
 }
 /* Check if this bit is 0 */
 else
 {
 /* Perform NRZ-I encoding */
 outbit = (~(buf->last_bit) & 0x01); /* Next bit is
inverse of previous */
 buf->last_bit = outbit;

 /* Perform scrambling according to G3RUH standard */
 if(scramble)
 {
lfsr_hi = (buf->lfsr & 0x00000001L);
 lfsr_lo = ((buf->lfsr >> 5) & 0x01L);
 lfsr_bit = lfsr_hi ^ lfsr_lo;

 scr_bit = outbit ^ lfsr_bit;
buf->byte |= (scr_bit << buf->bit_counter);
buf->lfsr >>= 1;
 if(scr_bit) buf->lfsr |= 0x00010000L;
 }
 else
 buf->byte |= (outbit << buf->bit_counter);

 buf->bit_counter++;
 buf->frame_buffer[buf->index] = buf->byte;
 buf->hi_count = 0;

 /* Check if we have one byte to add to buffer */
if(buf->bit_counter == 8)
 {
 buf->index++;

 buf->byte = 0x00;
 buf->bit_counter = 0;
 }
 }
 }
 }

 /* Buffer all header data and retain FCS and LFSR valus, since
 * this is the same for all frames. */
 void buffer_header(TX_FRAME_t *frame, TX_BUFFER_t *buf, uint8_t scramble)
 {
 uint8_t c, i;
/* Buffer header flag */
 byte2buf(frame->flag, FALSE, buf, scramble);

 /* Buffer address field */
 for(i=0; i<frame->address_bytes; i++)
 {
 c = frame->address[i];
 byte2buf(c, TRUE, buf, scramble);
 }

 /* Buffer control field */
 byte2buf(frame->control, TRUE, buf, scramble);

/* Buffer pid field */
 byte2buf(frame->pid, TRUE, buf, scramble);
 
 buf->_hi_count = buf->hi_count;
buf->_last_bit = buf->last_bit;
 buf->_index = buf->index;
 buf->_bit_counter = buf->bit_counter;
 buf->_byte = buf->byte;
 buf->_lfsr = buf->lfsr;
 }

 /* Send UI frame to buffer, and perform bit stuffing and NRZ-I
encoding. */
void buffer_frame(TX_FRAME_t *frame, TX_BUFFER_t *buf, uint8_t scramble)
 {
 uint8_t c, fcs_hi, fcs_lo;
 uint16_t j;

for(j=0; j<frame->info_bytes; j++)
 {
 c = frame->info[j];
 byte2buf(c, TRUE, buf, scramble);
 }


 /* Buffer FCS field */
 fcs_hi = frame->fcs & 0x00FF;
 fcs_lo = (frame->fcs & 0xFF00) >> 8;
 byte2buf(fcs_hi, TRUE, buf, scramble);
 byte2buf(fcs_lo, TRUE, buf, scramble);

 /* Buffer tail flag */
 byte2buf(frame->flag, FALSE, buf, scramble);
 }

 /* Clear all data from tx_buffer, but preamble and header data*/
 void clear_tx_buffer(TX_BUFFER_t *buf)
 {
 uint16_t i;
 for(i=(buf->_index); i<FRAME_L; i++)
{
 buf->frame_buffer[i] = 0;
 }

 buf->hi_count = buf->_hi_count;
 buf->last_bit = buf->_last_bit;
 buf->index = buf->_index;
 buf->bit_counter = buf->_bit_counter;
buf->byte = buf->_byte;
 buf->lfsr = buf->_lfsr;
 }

/* This function can be used to buffer addidtional bytesoutside the
 * AX.25 frame, as preamble and additional flags. Optional
G3RUH scrambling
 * can also be performed. The LFSR will be updated
 * accordingly.
 * NRZI-encoding will be performed, but not bit stuffing.
 */
 void buffer_byte(uint8_t byte, TX_BUFFER_t *buf, uint8_t scramble)
{
 uint8_t i, bit, bmask, outbit, scr_bit;
 uint8_t lfsr_hi, lfsr_lo, lfsr_bit;

 bmask = 0x01;
// out_byte = 0x00;

for(i=0; i<8; i++)
 {
 bit = (byte & bmask) >> i;
 bmask <<= 1;

if(bit)
 outbit = buf->last_bit; /* Next bit is same as previous
*/
 else

outbit = (~(buf->last_bit) & 0x01); /* Next bit is
inverse of previous */

 buf->last_bit = outbit;

 /* G3RUH scramble */
 if(scramble)
 {
 lfsr_hi = (buf->lfsr & 0x00000001L);
 lfsr_lo = ((buf->lfsr >> 5) & 0x01L);
lfsr_bit = lfsr_hi ^ lfsr_lo;
 scr_bit = outbit ^ lfsr_bit;
 buf->byte |= (scr_bit << buf->bit_counter);
 buf->lfsr >>= 1;
 if(scr_bit) buf->lfsr |= 0x00010000L;
 }
 else
 buf->byte |= (outbit << buf->bit_counter);

 buf->frame_buffer[buf->index] = buf->byte;
 buf->bit_counter++;

 /* Check if we have one byte to add to buffer */
 if(buf->bit_counter == 8)
 {
 buf->index++;
buf->byte = 0x00;
 buf->bit_counter = 0;
 }
 }
 }

/* *******************************************************
* Functions for Rx.
 * *******************************************************/

/* Initialize RX frame */
void init_rx_frame(RX_FRAME_t *frame)
{
 uint16_t i;

 for(i=0; i<ADDR_L; i++)
 {
 frame->address[i] = 0x00;
}
 frame->address_count = 1;
 frame->control = 0x00;
 frame->pid = 0x00;

 for(i=0; i<INFO_L; i++)
{
 frame->info[i] = 0x00;

 }

frame->info_bytes = 0;
 frame->fcs = 0xFFFF;
 frame->valid = FALSE;
 }

 /* Initialize RX buffer */
 void init_rx_buffer(RX_BUFFER_t *buf)
 {
 uint16_t i;

 buf->data_bytes = 0;

 for(i=0; i<MAX_PACKET; i++)
 {
 buf->data_buffer[i] = 0x00;
 }

 clear_rx_buffer(buf);
 }

 /* Clears all meta data used when decoding, retains the
decoded data */
 void clear_rx_buffer(RX_BUFFER_t *buf)
 {
 buf->byte = 0x00;
 buf->bmask = 0;
buf->flag_detected = FALSE;
 buf->data_detected = FALSE;
 buf->last_bit = 0x00;
 buf->lfsr = 0L;
 buf->hi_count = 0;
 buf->flag_count = 0;
 }

 /* Decodes received data bit for bit. Can be called from an
interrupt routine or by functions that evaluate the
 * received data bit by bit */
 void add_bit(uint8_t bit, RX_FRAME_t *frame, RX_BUFFER_t *buf,
uint8_t scramble)
 {
 uint8_t lfsr_hi, lfsr_lo, lfsr_bit, scr_bit, cur_bit;
 // for debug data to usart
 #ifdef DEBUG
 uint8_t status_led[2];
status_led[0] = 0xCB;
#endif

 /* Descramble */
 if(scramble)
{
 lfsr_hi = (buf->lfsr & 0x00000001L);
 lfsr_lo = ((buf->lfsr >> 5) & 0x01L);
lfsr_bit = lfsr_hi ^ lfsr_lo;
 scr_bit = bit ^ lfsr_bit;
 buf->lfsr >>= 1;
if(bit)
 buf->lfsr |= 0x00010000L;
 }
else
 scr_bit = bit;

 /* NRZ-I decode */
if(scr_bit)
 {
 cur_bit = buf->last_bit;
 // the unscrambled data can be read here
 #ifdef DEBUG
DEBUG_PORT1.OUT = 0x01;
 #endif

 }
 else
 {
 cur_bit = (~(buf->last_bit)) & 0x01;
#ifdef DEBUG
 // the unscrambled data can be read here
DEBUG_PORT1.OUT = 0x00;
 #endif
 }

 // the nrz-i decoded data can be read here
 #ifdef DEBUG
 DEBUG_PORT2.OUT = cur_bit & 0x01;
#endif

 /* Remove bitstuffing if necessary */
 if((buf->data_detected) && (buf->hi_count == 5) && (cur_bit== 0))
{
 buf->hi_count = 0;
 buf->last_bit = scr_bit;
 return;
 }
 /* Add decoded bit to LSB of MSB of byte. Since LSB is
transmitted first, the decoded bit will be LSB * after 8 received bits */
 if(cur_bit)
{
 buf->byte |= 0x80;
 buf->hi_count++;
 }
 else
 {
 buf->byte &= 0x7F;

 buf->hi_count = 0;
 }

 buf->last_bit = scr_bit;
 /* An AX.25 flag is detected, handle received data, byte wise*/
 if(buf->flag_detected)
 {
 buf->bmask++;
if(buf->bmask == 8)
 {
buf->bmask = 0;

/* Preamble detected, look for head flag again. */
 if(buf->byte == 0xFF)
 buf->flag_detected = FALSE;

 /* Consecutive AX.25 flags, dont process */
 /* First data byte detected */
if(buf->byte != 0x7E)
 {
 buf->data_detected = TRUE;
 }
else
 {
 /* Tail flag detected. Handle packet if above minimum
packet length and under maximum length.
 * Can filter out som possible bogus data */
 buf->data_detected = FALSE;
 if((buf->data_bytes > MIN_PACKET) && (buf->data_bytes <
MAX_PACKET))
 {
 #ifdef DEBUG
status_led[1] = 0xCE;
send_debug(status_led);
#endif
/* Call for frame decoding */
 if(decode_frame(frame, buf))
 {
 frame->valid = TRUE;
clear_rx_buffer(buf);
 return;
}
else
{
 frame->valid = FALSE;
 #ifdef DEBUG
 status_led[1] = 0xCD;
 send_debug(status_led);
 #endif
 }
 init_rx_buffer(buf);
 init_rx_frame(frame);

 }
 else
{
 buf->flag_detected = FALSE;
 buf->data_detected = FALSE;
buf->data_bytes = 0;

 }
 }

 /* Add received data bytes to buffer */
 if(buf->data_detected)
 {
 buf->data_buffer[buf->data_bytes] = buf->byte;
 buf->data_bytes++;
 }
 }
 }
 /* Received AX.25 flag. Prepare for bytewise data handling*/
if((buf->byte == 0x7E) && (buf->flag_detected == FALSE))
{
buf->flag_detected = TRUE;
}

/* Make space for new bit */
 buf->byte >>= 1;
 }

 /* Decode received buffer into an AX.25 UI frame */
 uint8_t decode_frame(RX_FRAME_t *rx_frame, RX_BUFFER_t *rx_buf)
 {
 if(verify_frame(rx_buf))
 {
 uint8_t i, j, more_addresses,valid;

 more_addresses = TRUE;
 valid = FALSE;
 i = 0;
 j = 0;

/* Find addresses */
 while(more_addresses)
 {
 for(i=0; i<6; i++)
 {
 rx_frame->address[j] = (rx_buf->data_buffer[j] >> 1);
 j++;
 }
rx_frame->address[j] = ((rx_buf->data_buffer[j]) & 0x1E)>> 1;
 if(rx_buf->data_buffer[j++] & 0x01)

 more_addresses = FALSE;
 else
 rx_frame->address_count++;
 }
 /* Find control and pid info */
 rx_frame->control = rx_buf->data_buffer[j++];
 rx_frame->pid = rx_buf->data_buffer[j++];

 /* Strict check for if received frame is UI frame */
 if((rx_frame->control) == 0x03 && (rx_frame->pid == 0xF0))
 valid = TRUE;

 i = 0;
 for(j=j; j<((rx_buf->data_bytes)-2); j++)
{
 rx_frame->info[i++] = rx_buf->data_buffer[j];
 rx_frame->info_bytes++;
 }

 rx_frame->fcs = rx_buf->data_buffer[j++];
 rx_frame->fcs |= ((rx_buf->data_buffer[j]) << 8);
 return TRUE;
 }
 else
 return FALSE;
 }

 /* Verifiy the integrity of received frame */
 uint8_t verify_frame(RX_BUFFER_t *rx_buf)
 {
 uint16_t sum, i;
 uint8_t fcs_lo, fcs_hi, _fcs_lo, _fcs_hi;

 sum = 0xFFFF;

 /* Calculate and verify the FCS */
 for(i=0; i<((rx_buf->data_bytes)-2); i++)
 {
 sum = calcFCS(rx_buf->data_buffer[i], sum);
 }

 fcs_hi = rx_buf->data_buffer[i++];
 fcs_lo = rx_buf->data_buffer[i];

 sum ^= 0xFFFF;
 _fcs_hi = sum & 0x00FF;
 _fcs_lo = (sum & 0xFF00) >> 8;

 if((fcs_hi == _fcs_hi) && (fcs_lo == _fcs_lo))
 return TRUE;
 else
return FALSE; }
