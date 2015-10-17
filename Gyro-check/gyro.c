/***********************************************************
	C file for ITG-3200 Gyro
	Contains functions for gyro initialization and operation
************************************************************/

#include "gyro.h"
#include "usart.h"

/*******************************************************
		Read contents of a register(Burst read)
********************************************************/
void read_burst(uint8_t reg)
{
	uint8_t ret1 = 0x00, ret2 = 0x00;
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_GenerateSTART(I2C1, ENABLE);
	I2C_SendData(I2C1, W_ADDR);
	I2C_SendData(I2C1, reg);
	small_delay();
	
	I2C_GenerateSTART(I2C1, ENABLE);
	I2C_SendData(I2C1, R_ADDR);
	small_delay();
	
	ret1 = I2C_ReceiveData(I2C1);
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);
	small_delay();
	
	ret2 = I2C_ReceiveData(I2C1);
	small_delay();
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	USART_SendData(USART2, (uint8_t) ret1);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
	
	USART_SendData(USART2, (uint8_t) ret2);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
}

/***********************************************************
			Write value into a gyro register
************************************************************/

void write(uint8_t reg, uint8_t data)
{
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_GenerateSTART(I2C1, ENABLE);
	small_delay();
	I2C_SendData(I2C1, W_ADDR);
	small_delay();
	I2C_SendData(I2C1, PWR_MGM_REG);
	small_delay();
	I2C_SendData(I2C1, 0x80);
	small_delay();
	I2C_GenerateSTOP(I2C1, ENABLE);	
} 


/************************************************************
			Read contents of gyro register(single read)
*************************************************************/
void read_single(uint8_t reg)
{
	uint8_t ret = 0x00;
	
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	I2C_GenerateSTART(I2C1, ENABLE);
	small_delay();
	I2C_SendData(I2C1, W_ADDR);
	small_delay();
	I2C_SendData(I2C1, reg);
	small_delay();
	
	I2C_GenerateSTART(I2C1, ENABLE);
	I2C_SendData(I2C1, R_ADDR);
	small_delay();
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	I2C_NACKPositionConfig(I2C1, I2C_NACKPosition_Current);
	
	ret = I2C_ReceiveData(I2C1);
	small_delay();
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	USART_SendData(USART2, (uint8_t) ret);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
 }



/****************************************************
		Check contents of WHO AM I register
*****************************************************/
void Gyro_Init(void)
{
	read_single(WHO_AM_I_REG);
}


/****************************************************
		
*****************************************************/
void Gyro_set_default(void)
{
	/*		Reset to defaults		*/
	write(PWR_MGM_REG, 0x80);
	
	/*			SMLPRT_DIV = 0		*/
	write(SMPLRT_DIV_REG, 0x00);
	
	/*		DLPF_CFG = 0, FS_SEL = 3		*/
	write(DLPF_FS_REG, 0x18);
	
	/*		Generate interrupt when device is ready or raw data ready		*/
	write(INT_CFG_REG, 0x05);
	
	/*			*/
	write(PWR_MGM_REG, 0x00);
}


/****************************************************
						Read temperature Value
*****************************************************/
void Temp_read(void)
{
	read_single(TEMP_OUT_H_REG);
}


/****************************************************
						Read X-axis Value
*****************************************************/
void Gyro_read_X(void)
{
	read_single(GYRO_XOUT_H_REG);	
}



/****************************************************
						Read Y-axis Value
*****************************************************/
void Gyro_read_Y(void)
{
	read_single(GYRO_YOUT_H_REG);
}



/****************************************************
						Read Z-axis Value
*****************************************************/
void Gyro_read_Z(void)
{
	read_burst(GYRO_ZOUT_H_REG);
}
 
void send_check(void)
{
	FlagStatus status = RESET;
	while(status == RESET){
		status =  I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE);
	}
}

void rec_check(void)
{
	FlagStatus status = RESET;
	while(status == RESET){
		status =  I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE);	
	}
}

