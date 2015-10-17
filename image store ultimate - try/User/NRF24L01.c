#include "stm32f4xx.h"
#include "config.h"
#include "NRF24L01.h"
#include <stdio.h>
#include <string.h>



#define	RX_DR			0x40
#define	TX_DS			0x20
#define	MAX_RT			0x10

u8 TX_ADDRESS[TX_ADR_WIDTH] = {0xb2,0xb2,0xb3,0xb4,0x01};  // ����һ����̬���͵�ַ



u8 RX_BUF[TX_PLOAD_WIDTH];

u8 TX_BUF[TX_PLOAD_WIDTH];

/*����MISO and MOSI SCLK Ϊ���ù��ܣ����죩���  SPI1  */
/*����SPI NRF24L01+Ƭѡ	GPIOB_PIN_15     	CSN   	ͨ���������ģʽ */
/*����SPI NRF24L01+ģʽѡ��	GPIOB_PIN_14    CE		ͨ���������ģʽ*/
/*����SPI NRF24L01+�ж��ź�	 GPIOB_PIN_13   IRQ		��������ģʽ*/



void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStruct;	 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(Open_SPIx_SCK_GPIO_CLK | Open_SPIx_MISO_GPIO_CLK | Open_SPIx_MOSI_GPIO_CLK,ENABLE);
	RCC_APB2PeriphClockCmd(Open_RCC_APB2Periph_SPIx,ENABLE);
	
	GPIO_PinAFConfig(Open_SPIx_SCK_GPIO_PORT, Open_SPIx_SCK_SOURCE,  Open_SPIx_MOSI_AF);
	GPIO_PinAFConfig(Open_SPIx_MISO_GPIO_PORT, Open_SPIx_MISO_SOURCE, Open_SPIx_MOSI_AF);
	GPIO_PinAFConfig(Open_SPIx_MOSI_GPIO_PORT, Open_SPIx_MOSI_SOURCE, Open_SPIx_MOSI_AF);
	
	GPIO_InitStructure.GPIO_Pin = Open_SPIx_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;  
	GPIO_Init(Open_SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Open_SPIx_MISO_PIN;
	GPIO_Init(Open_SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = Open_SPIx_MOSI_PIN;
	GPIO_Init(Open_SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

	SPI_I2S_DeInit(Open_SPIx);
	SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(Open_SPIx, &SPI_InitStruct);

	SPI_Cmd(Open_SPIx, ENABLE);

}

static void SPI_Send_byte(SPI_TypeDef* SPIx,u8 data)
{
	while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(Open_SPIx,data);

	while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_RXNE)==RESET);
	SPI_I2S_ReceiveData(Open_SPIx);
}

static u8 SPI_Receive_byte(SPI_TypeDef* SPIx,u8 data)
{
	while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(Open_SPIx,0x00);
	
	while(SPI_I2S_GetFlagStatus(Open_SPIx, SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData(Open_SPIx);
}

static void delay1us(u8 t)
{
	while(--t);
} 

/****��Ĵ���regдһ���ֽڣ�ͬʱ����״̬�ֽ�**************/
u8 SPI_RW_Reg(u8 reg,u8 value)
{
	u8 status;
	CSN(0);
	status=SPI_Receive_byte(SPI1,reg);   //select register  and write value to it
	SPI_Send_byte(SPI1,value);   
	CSN(1);
	return(status); 
}
/****��Ĵ���reg��һ���ֽڣ�ͬʱ����״̬�ֽ�**************/
u8 SPI_Read_Reg(u8 reg)
{
	u8 status;
	CSN(0);
	SPI_Send_byte(SPI1,reg);
	status=SPI_Receive_byte(SPI1,0);   //select register  and write value to it
	CSN(1);
	return(status);
}
/********����bytes�ֽڵ�����*************************/
u8 SPI_Read_Buf(u8 reg,u8 *pBuf,u8 bytes)
{
	u8 status,byte_ctr;
	CSN(0);
	status=SPI_Receive_byte(SPI1,reg);       
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
		pBuf[byte_ctr]=SPI_Receive_byte(SPI1,0);
	CSN(1);
	return(status);
}

/****************д��bytes�ֽڵ�����*******************/
u8 SPI_Write_Buf(u8 reg,u8 *pBuf,u8 bytes)
{
	u8 status,byte_ctr;
	CSN(0);
	status=SPI_Receive_byte(SPI1,reg); 
	delay1us(1);
	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
		SPI_Send_byte(SPI1,*pBuf++);
	CSN(1);
	return(status);
}

/*���պ���������1��ʾ�������յ�������û�����ݽ��յ�**/
u8 nRF24L01_RxPacket(u8* rx_buf)
{
    u8 status,revale=0;
	CE(0);
	delay1us(10);
	status=SPI_Receive_byte(SPI1,STATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��
//	CE(0);
//	status=0x40;
	printf("STATUS����״̬��0x%2x\r\n",status);

	if(status & RX_DR)				// �ж��Ƿ���յ�����
	{
//		CE(1);
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
//		CE(0);
		revale =1;			//��ȡ������ɱ�־
	}
	SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS,status);   //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
	CE(1);
	return revale;	
}

 /****************���ͺ���***************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE(0);			//StandBy Iģʽ	
	SPI_Write_Buf(WRITE_REG_NRF24L01 + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // װ������	
	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0e);   		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	CE(1);		 //�ø�CE���������ݷ���
	delay1us(10);
}

void RX_Mode(void)
{
	CE(0);
		printf("Configure is ready!\r\n");
  	SPI_Write_Buf(WRITE_REG_NRF24L01 + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RX_PW_P0, TX_PLOAD_WIDTH);  // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ�� 
 
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_AA, 0x3f);               // ʹ�ܽ���ͨ��0�Զ�Ӧ��
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_RXADDR, 0x3f);           // ʹ�ܽ���ͨ��0
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_CH, 40);                 // ѡ����Ƶͨ��0x40

  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_SETUP, 0x07);            // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0f);              // CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
  	CE(1);
}

void TX_Mode(u8 * tx_buf)
{
	CE(0);
	printf("Configure is ready!\r\n");
  	SPI_Write_Buf(WRITE_REG_NRF24L01 + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // д�뷢�͵�ַ
  	SPI_Write_Buf(WRITE_REG_NRF24L01 + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
  	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // װ������
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_AA, 0x3f);       // ʹ�ܽ���ͨ��0�Զ�Ӧ��
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_RXADDR, 0x3f);   // ʹ�ܽ���ͨ��0
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + SETUP_RETR, 0x0a);  // �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_CH, 40);         // ѡ����Ƶͨ��0x40
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_SETUP, 0x07);    // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RX_PW_P0, TX_PLOAD_WIDTH);  // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ��
  	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0e);      // CRCʹ�ܣ�16λCRCУ�飬�ϵ�
	CE(1);
	delay1us(10);
} 

void nRF24L01_Initial(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
 SPI_Configuration();
	 
	printf("SPI is ready!\r\n");

  RCC_AHBxPeriphClockCmd(RCC_GPIO_CS,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_PIN_CS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIO_CS_PORT, &GPIO_InitStructure);
	
//	CE(0);			//nRF24L01_CE=0;			 chip enable
//	nRF24L01_Config();

//	CSN(0);			//nRF24L01_CSN=1;			 Spi enable
	//Initial_SPI(SPI1);
	SPI_Configuration();
}

/****************** ���ú���********************************/
void nRF24L01_Config(void)
{
          //initial io
	//CE(0);          //        CE=0 ;chip enable
	//CSN(1);       //CSN=1   Spi disable
	SPI_RW_Reg(WRITE_REG_NRF24L01 + CONFIG, 0x0e); // Set PWR_UP bit, enable CRC(2 bytes) &Prim:RX. RX_DR enabled..
	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_AA, 0x3f);
	SPI_RW_Reg(WRITE_REG_NRF24L01 + EN_RXADDR, 0x3f); // Enable Pipe0
//	SPI_RW_Reg(WRITE_REG_NRF24L01 + SETUP_AW, 0x02); // Setup address width=5 bytes
//	SPI_RW_Reg(WRITE_REG_NRF24L01 + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_CH, 40);
	SPI_RW_Reg(WRITE_REG_NRF24L01 + RF_SETUP,0x07); // TX_PWR:0dBm, Datarate:2Mbps,
}
 
void NRF24L01_Send(void)
{
    u8 status=0x00;
//	CSN(0);
	TX_Mode(TX_BUF);
	while(IRQ);

	CE(0);
	delay1us(10);
	status=SPI_Read_Reg(STATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��
	if(status&TX_DS)	/*tx_ds == 0x20*/
	{
		printf("STATUS Readyx%2x\r\n",status);
		printf("\r\nTransmit ready %s\r\n",RX_BUF);	
		SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, 0x20);      // ���TX����IRQ���ͣ�
			
	}
	else if(status&MAX_RT)
		{
			printf("Check Right");	
			SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, 0x10);      // ���TX����IRQ���ͣ�			
		}
	CE(1);
//	status=20;
}


void NRF24L01_Receive(void)
{   
  u8 status,i =0x01;  
	SPI_Configuration();
	RX_Mode();
	while(IRQ);
//	printf("�����ж�\n");
	CE(0);
	delay1us(10);
	status=SPI_Read_Reg(STATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��
	printf("STATUS Ready x%2x\r\n",status);
	if(status & 0x60)			//�����жϱ�־λ
	{
		printf("Ready Work");
//		CE(1);
		SPI_Read_Buf(RD_RX_PLOAD,RX_BUF,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer

		printf("\r\n i=%d,Payload Rx%x\r\n",i,RX_BUF[0]);
//		for(i=0;i<32;i++)
//		{
//			RX_BUF[1] = 0X06;
//			printf("\r\n i=%d,���յ����ݣ�%x\r\n",i,RX_BUF[i]);
//		}
		SPI_RW_Reg(WRITE_REG_NRF24L01 + STATUS, 0x40);      // ���TX����IRQ����
	}  
	CE(1);

}
