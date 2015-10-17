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
char FLASHStatus(void);
void BUFRead(unsigned char *data, unsigned int addr, unsigned int length, unsigned char buf);
void BUFWrite(unsigned char *data, unsigned int addr, unsigned int length, unsigned char buf);
void MainWrite(unsigned char *data, unsigned int page_addr, unsigned int byte_addr, unsigned int length, unsigned char buf);
void Buf2MainWrite (unsigned int addr, unsigned char buf);
void PageRead (unsigned char* data, unsigned int page_addr, unsigned int byte_addr, unsigned int length);
void Main2BufRead(unsigned int page_addr, unsigned char buf);
 void AT45DBXX_Read_sec(unsigned char *sec);
  void AT45DBXX_Read_sec_lock(unsigned char *sec);
	unsigned char Status_read(void);

// External Functions and Data structures
//typedef struct aa{
//unsigned int pageAddr : 12;
//unsigned int byteAddr : 10;
//} flashPtr;

typedef unsigned char BYTE;
//BYTE buffer[PAGESIZE] = {0};
void writeFlash(BYTE *buffer, unsigned int page_addr, unsigned int byte_addr);
void readFlash(BYTE *buffer, unsigned int page_addr, unsigned int byte_addr);
//Page is 528 bytes, buffer is 528 bytes
#define BUFFER1 1
#define BUFFER2 2
//Status Read
#define STATUS_READ 0xD7 //0x57
#define SPI_BUSY ((SPSR & 0x80)==0)
// DEFINE OPCODES FOR FLASH FUNCTIONS
//Buffer Read
//followed by 14 don't care bits, 10 byte address, 8 don't care - wraps around
#define BUF1_READ 0xD4 //0x54
#define BUF2_READ 0xD6 //0x56
//Buffer writes
//followed by 14 don't care bits, 10 byte address - write continues with wrap-around until CS HIGH
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
//followed by 2 don't care, 12 page address, 10 byte address, 32 don't care
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
//followed by 2 don't care, 12 page address, 10 byte address, 32 don't care
//additional clock pulses read data while CS low
#define CONT_READ 0xE8 //0x68
//FLASH INTERACTION FUNCTIONS
char FLASHStatus(void) {
	int i;
//AT45DBXX_BUSY();
	AT45DBXX_Enable; //CS LOW
	SPI_Send_byte(STATUS_READ);  //opcode for read status
   i=SPI_Receive_byte();
AT45DBXX_Disable; //end
return i;
}

void BUFRead(unsigned char *data, unsigned int addr, unsigned int length, unsigned char buf)
{
unsigned char i;
AT45DBXX_Enable; //CS LOW
if (buf==BUFFER1)
{
SPI_Send_byte( BUF1_READ); //opcode for read status
}
if (buf==BUFFER2)
{
SPI_Send_byte(BUF2_READ);
}
SPI_Send_byte(0x00); //don't care

SPI_Send_byte((char)(addr>>8)); //don't care and 1st 2 addr bits

SPI_Send_byte((char)(addr)); //address

SPI_Send_byte(0x00); //don't care

for(i=0;i<length;i++)
{
*data = SPI_Receive_byte();
data++;
}
AT45DBXX_Disable; //end
}

void BUFWrite(unsigned char *data, unsigned int addr, unsigned int length, unsigned char buf)
{
unsigned int i;
AT45DBXX_Enable;//CS LOW
if (buf==BUFFER1)
{
SPI_Send_byte( BUF1_WRITE); //opcode
}
if (buf==BUFFER2)
{
SPI_Send_byte(BUF2_WRITE);
}

SPI_Send_byte(0xff); //don't care
 //wait for transmission complete
SPI_Send_byte((char)(addr>>8)); //don't care and 1st 2 addr bits

SPI_Send_byte((char)(addr)); //address 0

for (i=0;i<length;i++)
{
SPI_Send_byte( data[i]); //data
//data++;
}
AT45DBXX_Disable;
}


void MainWrite(unsigned char *data, unsigned int page_addr, unsigned int byte_addr, unsigned int length, unsigned char buf)
{
unsigned int i;
AT45DBXX_Enable;//CS LOW
if (buf==BUFFER1)
{
SPI_Send_byte(	MAIN_WRITE1); //opcode for read status
}
if (buf==BUFFER2)
{
SPI_Send_byte(MAIN_WRITE2);
}
 //wait for transmission complete
SPI_Send_byte((char)(page_addr>>7));
 //wait for transmission complete
SPI_Send_byte((char)(page_addr<<1)|(0x01 & (byte_addr>>8)));

SPI_Send_byte((char)(byte_addr));

for (i=0;i<length;i++)
{
SPI_Send_byte(*data); //data
data++;
}
AT45DBXX_Disable; //end
}

void Buf2MainWrite (unsigned int addr, unsigned char buf)
{
AT45DBXX_Enable; //CS LOW
if (buf==BUFFER1)
{
SPI_Send_byte(BUF1_TO_MAIN_WRITE);
}
if (buf==BUFFER2)
{
SPI_Send_byte(BUF2_TO_MAIN_WRITE);
}

SPI_Send_byte((char)(addr>>7));

SPI_Send_byte((char)(addr<<1));

SPI_Send_byte(0x00); //don't care;

AT45DBXX_Disable; //end

}

void PageRead (unsigned char *data, unsigned int page_addr, unsigned int byte_addr, unsigned int length)
{
unsigned int i;
AT45DBXX_Enable; //end

SPI_Send_byte( PAGE_READ);
SPI_Send_byte((char)(page_addr>>7));

SPI_Send_byte((char)(page_addr<<1)|(0x01 & (byte_addr>>8)));

SPI_Send_byte((char)(byte_addr));

//32 don't care bits
for (i=0;i<4;i++)
{SPI_Send_byte( 0x00);

}
for (i=0;i<length;i++)
{
*data =SPI_Receive_byte();
data++;
}
AT45DBXX_Disable;
}


void Main2BufRead(unsigned int page_addr, unsigned char buf) {
AT45DBXX_Enable; //CS LOW
if (buf==BUFFER1)
{
SPI_Send_byte(MAIN_TO_BUF1_READ); //opcode for read status
}
if (buf==BUFFER2)
{
SPI_Send_byte(	MAIN_TO_BUF2_READ);
} //wait for transmission complete
SPI_Send_byte((char)(page_addr>>7)); //2 reserve, then 6 page_addr bits, PA11-PA6
 //wait for transmission complete
SPI_Send_byte( (char)(page_addr<<1)); //6 addr bits and 2 don't care

SPI_Send_byte(0x00); // 8 don't cares


AT45DBXX_Disable; //end
}

void PageErase(unsigned int page_addr) {
AT45DBXX_Enable; //CS LOW
//while (SPI_BUSY);
SPI_Send_byte((char)(page_addr>>7));
SPI_Send_byte((char)(page_addr<<1));

	SPI_Send_byte(0x00); //don't care;

AT45DBXX_Disable;
}


void BlockErase(unsigned int block_addr) {
AT45DBXX_Enable; //CS LOW
//while (SPI_BUSY);
 SPI_Send_byte((char)(block_addr>>3));

SPI_Send_byte((char)(block_addr<<5));

SPI_Send_byte(0x00); //don't care;

AT45DBXX_Disable;
}

void SectorErase(unsigned int sector_addr)
{
AT45DBXX_Enable; //CS LOW


SPI_Send_byte((char)(sector_addr<<2));
//while (SPI_BUSY);
SPI_Send_byte(0x00); //don't care;
//while (SPI_BUSY);
SPI_Send_byte( 0x00); //don't care;

AT45DBXX_Disable;
}

void writeFlash(BYTE *buffer,unsigned int page_addr, unsigned int byte_addr) {
MainWrite(buffer, page_addr, byte_addr,PAGESIZE, BUFFER1);
}

void readFlash(BYTE *buffer, unsigned int page_addr, unsigned int byte_addr) {
PageRead(buffer,page_addr, byte_addr, PAGESIZE);
}

 void AT45DBXX_Read_sec(unsigned char *sec)
{ u8 i;
  AT45DBXX_Enable;
	SPI_Send_byte(0x32);
SPI_Send_byte(0x00); 
SPI_Send_byte(0x00); 
SPI_Send_byte(0x00); 	
	for(i=0;i<8;i++)
	sec[i]=SPI_Receive_byte();
  AT45DBXX_Disable;	
}
 void AT45DBXX_Read_sec_lock(unsigned char *sec)
{ u8 i;
  AT45DBXX_Enable;
	SPI_Send_byte(0x35);
SPI_Send_byte(0x00); 
SPI_Send_byte(0x00); 
SPI_Send_byte(0x00); 	
	for(i=0;i<8;i++)
	sec[i]=SPI_Receive_byte();
AT45DBXX_Disable;	
}

unsigned char Status_read(void)
{ u8 i;
	  AT45DBXX_Enable;
   SPI_Send_byte( STATUS_READ); 

		do
	{i = SPI_Receive_byte();}
	while(!(i & 0x80));
AT45DBXX_Disable;
	return i;

}
