/*
Flash header file
Two functions used to interface with flash:
writeFlash(BYTE * buffer, flashPtrwriteLoc);
readFlash(BYTE * buffer, flashPtrreadLoc);
USED FOR WRITING AND READING:
BYTE * buffer is an array of size PAGESIZE (528)
buffer is NOT erased after reads and writes
USED FOR ADDRESSING:
typedefstruct {
unsignedintpage_addr : 12;
unsignedintbyte_addr : 10;
} flashPtr;
*/
/*
#define RXB8 1
#define TXB8 0
#define UPE 2
#define OVR 3
#define FE 4
#define UDRE 5
#define RXC 7
*/
/*
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<UPE)
#define DATA_OVERRUN (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
*/
// Page size
#define PAGESIZE 528
//BEGINNING PAGE NUMBERS OF EACH DATA TYPE
#define UPLINK_START 1024
#define TELEM_START 1280
#define LOG_START 2304
#define IMAGE1_START 2560
#define IMAGE2_START 3328
// Internal Functions
charFLASHStatus(void);
void BUFRead(unsigned char *data, unsigned intaddr, unsigned char length, unsigned char buf);
void BUFWrite(unsigned char *data, unsigned intaddr, unsigned char length, unsigned char buf);
void MainWrite(unsigned char *data, unsigned intpage_addr, unsigned int byte_addr, unsigned char length, unsigned char buf);
void Buf2MainWrite (unsigned int addr, unsigned char buf);
void PageRead (unsigned char* data, unsigned int page_addr, unsigned int byte_addr, unsigned char length);
void Main2BufRead(unsigned intpage_addr, unsigned char buf);
// External Functions and Data structures
typedef struct {
unsignedintpageAddr : 12;
unsignedintbyteAddr : 10;
} flashPtr;
//BYTE buffer[PAGESIZE] = {0};
voidwriteFlash(BYTE * buffer, flashPtrwriteLoc);
voidreadFlash(BYTE * buffer, flashPtrreadLoc);
//Page is 528 bytes, buffer is 528 bytes
#define BUFFER1 1
#define BUFFER2 2
//Status Read
#define STATUS_READ 0xD7 //0x57
#define SPI_BUSY ((SPSR & 0x80)==0)
// DEFINE OPCODES FOR FLASH FUNCTIONS
//Buffer Read
//followed by 14 don't care bits, 10 byte address, 8 don't care - wraps
around
#define BUF1_READ 0xD4 //0x54
#define BUF2_READ 0xD6 //0x56
//Buffer writes
//followed by 14 don't care bits, 10 byte address - write continues
with wrap-around until CS HIGH
#define BUF1_WRITE 0x84
#define BUF2_WRITE 0x87
//Main Memory Page through Buffer with erase
//followed by 2 don't care bits, 12 page address, 10 byte address
#define MAIN_WRITE1 0x82
#define MAIN_WRITE2 0x85
//Buffer to Main mem
//followed by 2 don't care bits, 12 page address, 10 don't care
//on CS High - erases page then writes
#define BUF1_TO_MAIN_WRITE 0x83
#define BUF2_TO_MAIN_WRITE 0x86
//if desired, same function but it assumes page has already been erased
//currently, these function are not nedded
#define BUF1_TO_MAIN_NO_ERASE 0x88
#define BUF2_TO_MAIN_NO_ERASE 0x89
//Main Mem Read
//followed by 2 don't care, 12 page address, 10 byte address, 32 don't
care
#define PAGE_READ 0xD2 //0x52
// Main read to Buffers
//followed by 2 don't care bits, 12 page address, 10 don't care
#define MAIN_TO_BUF1_READ 0x53
#define MAIN_TO_BUF2_READ 0x55
//Page Erase
//followed by 2 don't care bits, 12 page address, 10 don't care
//erases to 1s on CS HIGH
#define PAGE_ERASE 0x81
//Block Erase
//followed by 2 don't care bits, 9 block address, 13 don't care
//erases block of 8 pages to 1 on CS HIGH
#define BLOCK_ERASE 0x50
//Sector Erase
//followed by 2 don't care bits, 4 sector address, 18 don't care
//erases sector of 256 pages to 1 on CS HIGH
#define SECTOR_ERASE 0x7C
//Currently unused potential function
//Continuous Array Read
//followed by 2 don't care, 12 page address, 10 byte address, 32 don't
care
//additional clock pulses read data while CS low
#define CONT_READ 0xE8 //0x68
//FLASH INTERACTION FUNCTIONS
charFLASHStatus(void) {
PORTB &= ~0x01; //CS LOW
SPDR = 0xD7; //opcode for read status
while (SPI_BUSY); //wait for transmission complete
SPDR = 0x00; //read SPDR
while(SPI_BUSY); //wait
PORTB |= 0x01; //end
return SPDR;
}
#define BUF2_TO_MAIN_WRITE 0x86
//if desired, same function but it assumes page has already been erased
//currently, these function are not nedded
#define BUF1_TO_MAIN_NO_ERASE 0x88
#define BUF2_TO_MAIN_NO_ERASE 0x89
//Main Mem Read
//followed by 2 don't care, 12 page address, 10 byte address, 32 don't
care
#define PAGE_READ 0xD2 //0x52
// Main read to Buffers
//followed by 2 don't care bits, 12 page address, 10 don't care
#define MAIN_TO_BUF1_READ 0x53
#define MAIN_TO_BUF2_READ 0x55
//Page Erase
//followed by 2 don't care bits, 12 page address, 10 don't care
//erases to 1s on CS HIGH
#define PAGE_ERASE 0x81
//Block Erase
//followed by 2 don't care bits, 9 block address, 13 don't care
//erases block of 8 pages to 1 on CS HIGH
#define BLOCK_ERASE 0x50
//Sector Erase
//followed by 2 don't care bits, 4 sector address, 18 don't care
//erases sector of 256 pages to 1 on CS HIGH
#define SECTOR_ERASE 0x7C
//Currently unused potential function
//Continuous Array Read
//followed by 2 don't care, 12 page address, 10 byte address, 32 don't
care
//additional clock pulses read data while CS low
#define CONT_READ 0xE8 //0x68
//FLASH INTERACTION FUNCTIONS
charFLASHStatus(void) {
PORTB &= ~0x01; //CS LOW
SPDR = 0xD7; //opcode for read status
while (SPI_BUSY); //wait for transmission complete
SPDR = 0x00; //read SPDR
while(SPI_BUSY); //wait
PORTB |= 0x01; //end
return SPDR;
}
void BUFRead(unsigned char *data, unsigned intaddr, unsigned char length, unsigned char buf)
{
unsigned char i;
PORTB &= ~0x01; //CS LOW
if (buf==BUFFER1)
{
SPDR = BUF1_READ; //opcode for read status
}
if (buf==BUFFER2)
{
SPDR = BUF2_READ;
}
while (SPI_BUSY); //wait for transmission complete
SPDR = 0x00; //don't care
while (SPI_BUSY); //wait for transmission complete
SPDR = (char)(addr>>8); //don't care and 1st 2 addr bits
while (SPI_BUSY);
SPDR = (char)(addr); //address
while (SPI_BUSY);
SPDR = 0x00; //don't care
while (SPI_BUSY);
for(i=0;i<length;i++)
{
SPDR = 0x00; //read
while (SPI_BUSY);
*data = SPDR;
data++;
}
PORTB |= 0x01; //end
}

void BUFWrite(unsigned char *data, unsigned intaddr, unsigned char length, unsigned char buf)
{
unsigned char i;
PORTB &= ~0x01; //CS LOW
if (buf==BUFFER1)
{
SPDR = BUF1_WRITE; //opcode for read status
}
if (buf==BUFFER2)
{
SPDR = BUF2_WRITE;
}
while (SPI_BUSY); //wait for transmission complete
SPDR = 0x00; //don't care
while (SPI_BUSY); //wait for transmission complete
SPDR = (char)(addr>>8); //don't care and 1st 2 addr bits
while (SPI_BUSY);
SPDR = (char)(addr); //address 0
while (SPI_BUSY);
for (i=0;i<length;i++)
29
{
SPDR = *data; //data
while (SPI_BUSY);
data++;
}
PORTB |= 0x01; //end
}
voidMainWrite(unsigned char *data, unsigned intpage_addr, unsigned
intbyte_addr, unsigned char length, unsigned char buf)
{
unsigned char i;
PORTB &= ~0x01; //CS LOW
if (buf==BUFFER1)
{
SPDR = MAIN_WRITE1; //opcode for read status
}
if (buf==BUFFER2)
{
SPDR = MAIN_WRITE2;
}
while (SPI_BUSY); //wait for transmission complete
SPDR = (char)(page_addr>>6);
while (SPI_BUSY); //wait for transmission complete
SPDR = (char)((page_addr<<2) | ((0x0300 &byte_addr)>>8));
while (SPI_BUSY);
SPDR = (char)(byte_addr);
while (SPI_BUSY);
for (i=0;i<length;i++)
{
SPDR = *data; //data
while (SPI_BUSY);
data++;
}
PORTB |= 0x01; //end
}
void Buf2MainWrite (unsigned intaddr, unsigned char buf)
{
PORTB &= ~0x01; //CS LOW
if (buf==BUFFER1)
{
SPDR = BUF1_TO_MAIN_WRITE;
}
if (buf==BUFFER2)
{
SPDR = BUF2_TO_MAIN_WRITE;
}
while (SPI_BUSY);
SPDR = (char)(addr>>6);
while (SPI_BUSY);
SPDR = (char)(addr<<2);
while (SPI_BUSY);
SPDR = 0x00; //don't care;
while (SPI_BUSY);
PORTB |=0x01;
}
voidPageRead (unsigned char* data, unsigned intpage_addr, unsigned
intbyte_addr, unsigned char length)
{
unsigned char i;
PORTB &= ~0x01;
SPDR = PAGE_READ;
while (SPI_BUSY);
SPDR = (char)(page_addr>>6);
while (SPI_BUSY);
SPDR = (char)((page_addr<<2) | ((0x0300 &byte_addr)>>8));
while (SPI_BUSY);
SPDR = (char)(byte_addr);
while (SPI_BUSY);
//32 don't care bits
for (i=0;i<4;i++)
{
SPDR = 0x00;
while (SPI_BUSY);
}
for (i=0;i<length;i++)
{
SPDR = 0x00; //read
while (SPI_BUSY);
*data = SPDR;
data++;
}
PORTB |=0x01;
}
void Main2BufRead(unsigned intpage_addr, unsigned char buf) {
PORTB &= ~0x01; //CS LOW
if (buf==BUFFER1)
{
SPDR = MAIN_TO_BUF1_READ; //opcode for read status
}
if (buf==BUFFER2)
{
SPDR = MAIN_TO_BUF2_READ;
}
while (SPI_BUSY); //wait for transmission complete
SPDR = (char)(page_addr>>6); //2 reserve, then 6 page_addr bits,
PA11-PA6
while (SPI_BUSY); //wait for transmission complete
SPDR = (char)(page_addr<<2); //6 addr bits and 2 don't care
while (SPI_BUSY);
SPDR = 0x00; // 8 don't cares
while (SPI_BUSY);

PORTB |= 0x01; //end
}
voidPageErase(unsigned intpage_addr) {
PORTB &= ~0x01; //CS LOW
while (SPI_BUSY);
SPDR = (char)(page_addr>>6);
while (SPI_BUSY);
SPDR = (char)(page_addr<<2);
while (SPI_BUSY);
SPDR = 0x00; //don't care;
while (SPI_BUSY);
PORTB |=0x01;
}
voidBlockErase(unsigned intblock_addr) {
PORTB &= ~0x01; //CS LOW
while (SPI_BUSY);
SPDR = (char)(block_addr>>3);
while (SPI_BUSY);
SPDR = (char)(block_addr<<5);
while (SPI_BUSY);
SPDR = 0x00; //don't care;
while (SPI_BUSY);
PORTB |=0x01;
}
voidSectorErase(unsigned intsector_addr) {
PORTB &= ~0x01; //CS LOW
while (SPI_BUSY);
SPDR = (char)(block_addr<<2);
while (SPI_BUSY);
SPDR = 0x00; //don't care;
while (SPI_BUSY);
SPDR = 0x00; //don't care;
while (SPI_BUSY);
PORTB |=0x01;
}
voidwriteFlash(BYTE * buffer, flashPtr * writeLoc) {
MainWrite(buffer, writeLoc.page_addr,
writeLoc.byte_addr,PAGESIZE, BUFFER1);
}
voidreadFlash(BYTE * buffer, flashPtr * readLoc) {
PageRead(buffer, readLoc.page_addr, readLoc.byte_addr, PAGESIZE);
Selection from cucp_ex.c
if (Net_Msg_Status == MSG_EBSRIE) { // Command and payload have been
received
// continue CUCP data transfer
Net_Msg_Status = MSG_READ;
// 11/27/06 all commands included, but need to actually fill in
actions for each case
switch (Net_Cmd) {
case NET_CMD_PING:
break;
case NET_CMD_GET_STATUS:
send((BYTE32 *) &myStatus, RESP_STATUS_SIZE);
break;
case NET_CMD_SILENCE:
break;
case NET_CMD_REPROGRAM:
break;
case NET_CMD_SET_TIME:
break;
case NET_CMD_TC_CMDS_AVAIL:
break;
case NET_CMD_TC_GET_CMD:
break;
case NET_CMD_TC_GET_PAYLOAD:
break;
case NET_CMD_TC_CMD_RESULT:
// Temporary TCAP testing cmd.
tcapActive = 1;
break;
case NET_CMD_TC_CMD_RESULT_PLD:
break;
case NET_CMD_TC_TELEM_BUFFER:
break;
case NET_CMD_TC_IMI_BUFFER:
break;
case NET_CMD_TC_IMAGE_METADATA:
break;
case NET_CMD_TC_IMAGE_CHUNK:

break;
case NET_CMD_TC_LOG_BUFFER:
break;
case NET_CMD_TC_CROSS_SEND:
//xmitData(&myCrossData,CROSS_GPS_DATA_SIZE);
xmitData(&myCrossGPS,128); //hack
break;
case NET_CMD_TC_CHECK_NEW_CODE:
break;
case NET_CMD_TC_GIVE_NEW_CODE:
break;
case NET_CMD_TC_GET_TELEM:
send((BYTE32 *) &myTelem, RESP_TC_TELEM_SIZE);
break;
case NET_CMD_TC_REPORT_ERROR:
break;
case NET_CMD_TC_GET_CROSS_GPS:
PORTC ^= 0x1;
/* Send received GPS from Cross to hypertrm */
xmitData(buffer,10);
if(GPStoTransfer == 1)
{
xmitData(buffer,10);
}
break;
case NET_CMD_TC_GPS_CROSS_BUFFER:
{
inti = 0;
for( ; i<10; i++)
{
buffer[i] = i+0x30; // this way it looks like ascii
numbers
}
}
GPStoTransfer = 1;
/* Receive data from CDH and signal that you have GPS data
to send to other satelltie */
break;
case NET_CMD_TC_CROSS_GPS_RELD:
break;
case NET_CMD_TC_STOP_DOWNLINK:
break;
default:
break;
}

} //if (Net_Msg_Status == MSG_EBSRIE)
}
}
BYTE* get_receive_mem(BYTE Net_Cmd, UINT* memBufferSize, BYTE*
hasResponse, BYTE* isValidCmd) {
*isValidCmd = 1;
switch (Net_Cmd) {
case NET_CMD_PING:
*memBufferSize = 0;
*hasResponse = 0;
return NULL;
case NET_CMD_GET_STATUS:
*memBufferSize = 0;
*hasResponse = 1;
return NULL;
case NET_CMD_SILENCE:
*memBufferSize = 0;
*hasResponse = 0;
return NULL;
case NET_CMD_REPROGRAM:
*memBufferSize = REPROGRAM_PAGE_SIZE;
*hasResponse = 0;
return (BYTE32*) &myReprogram;
case NET_CMD_SET_TIME:
*memBufferSize = TIMESTAMP_SIZE; //sizeof(MyTime);
*hasResponse = 0;
return (BYTE32*) &myTime;
// edited 11/28/06 to include all current CUCP commands
case NET_CMD_TC_CMDS_AVAIL:
//*memBufferSize = RESP_TC_CMDS_AVAIL_SIZE;
//sizeof(RESP_TC_CMDS_AVAIL);
*memBufferSize = 0;
*hasResponse = 1;
return NULL;
case NET_CMD_TC_GET_CMD:
//*memBufferSize = RESP_TC_GET_CMD_SIZE;
*memBufferSize = 0;
*hasResponse = 1;
return NULL;
case NET_CMD_TC_GET_PAYLOAD:
*memBufferSize = 0;
*hasResponse = 1;

return NULL;
case NET_CMD_TC_CMD_RESULT:
*memBufferSize = PLD_CMD_RESP_SIZE;
*hasResponse = 0;
return (BYTE32*) &myCommandResponse;
//not sure how to isolate payload from PLD_CMD_RESP
case NET_CMD_TC_CMD_RESULT_PLD:
//*memBufferSize= ;
*hasResponse = 0;
return;
case NET_CMD_TC_TELEM_BUFFER:
*memBufferSize = PLD_TC_TELEM_SIZE;
*hasResponse = 0;
return (BYTE32*) &myTelemBuffer;
case NET_CMD_TC_IMI_BUFFER:
*memBufferSize = IMI_DATA_SIZE;
*hasResponse = 0;
return (BYTE32*) &myIMI;
case NET_CMD_TC_IMAGE_METADATA:
*memBufferSize = PLD_TC_METADATA_SIZE;
*hasResponse = 0;
return (BYTE32*) &myMetadata;
case NET_CMD_TC_IMAGE_CHUNK:
*memBufferSize = PLD_TC_IMAGE_SIZE;
*hasResponse = 0;
return (BYTE32*) &myImage;
//Don't see LOG_BUFFER in cucp_def_data.h
case NET_CMD_TC_LOG_BUFFER:
//*memBufferSize= ;
*hasResponse = 0;
//return (BYTE32*) &;
case NET_CMD_TC_CROSS_SEND:
*memBufferSize = 0;
*hasResponse = 0;
return NULL;
case NET_CMD_TC_CHECK_NEW_CODE:
*memBufferSize = 0;
*hasResponse = 1;
return NULL;
case NET_CMD_TC_GIVE_NEW_CODE:
*memBufferSize = 0;
*hasResponse = 1;
return NULL;
case NET_CMD_TC_GET_TELEM:
*memBufferSize = 0;

*hasResponse = 1;
return NULL;
case NET_CMD_TC_REPORT_ERROR:
*memBufferSize = PLD_REPORT_ERROR_SIZE;
*hasResponse = 0;
return (BYTE32*) &myError;
case NET_CMD_TC_GET_CROSS_GPS:
*memBufferSize = 0;
*hasResponse = 1;
return NULL;
case NET_CMD_TC_GPS_CROSS_BUFFER:
*memBufferSize = CROSS_GPS_DATA_SIZE;
*hasResponse = 0;
return (BYTE32*) &myCrossGPS;
case NET_CMD_TC_CROSS_GPS_RELD:
*memBufferSize = GPS_RELDIST_SIZE;
*hasResponse = 0;
return (BYTE32*) &myGPSDist;
case NET_CMD_TC_STOP_DOWNLINK:
*memBufferSize = 0;
*hasResponse = 0;
return NULL;
default:
*isValidCmd = 0;
return NULL;
