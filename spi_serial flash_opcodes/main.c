/**
  ******************************************************************************
  * @file    RCC/main.c 
  * @author  payload team, studsat-2
  * @version V1.0.0
  * @date    13/1/2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#define SPI_PORT                  SPI2
#define SPI_PORT_CLOCK            RCC_APB1Periph_SPI2
#define SPI_PORT_CLOCK_INIT       RCC_APB1PeriphClockCmd

#define SPI_NSS_PIN              GPIO_Pin_12
#define SPI_NSS_GPIO_PORT        GPIOB
#define SPI_NSS_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define SPI_NSS_SOURCE           GPIO_PinSource12
#define SPI_NSS_AF               GPIO_AF_SPI2

 
#define SPI_SCK_PIN              GPIO_Pin_13
#define SPI_SCK_GPIO_PORT        GPIOB
#define SPI_SCK_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define SPI_SCK_SOURCE           GPIO_PinSource13
#define SPI_SCK_AF               GPIO_AF_SPI2
 
#define SPI_MOSI_PIN             GPIO_Pin_15
#define SPI_MOSI_GPIO_PORT       GPIOB
#define SPI_MOSI_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define SPI_MOSI_SOURCE          GPIO_PinSource15
#define SPI_MOSI_AF              GPIO_AF_SPI2

#define SPI_MISO_PIN             GPIO_Pin_14
#define SPI_MISO_GPIO_PORT       GPIOB
#define SPI_MISO_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define SPI_MISO_SOURCE          GPIO_PinSource14
#define SPI_MISO_AF              GPIO_AF_SPI2


/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup RCC_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
/* Private function prototypes -----------------------------------------------*/
void Delay (uint32_t nCount);
void HardwareSPI_init(void);
void spiPutWord(uint16_t data);
uint16_t spiGetWord(void) ;
uint16_t a;
//uint16_t datawrite[10]={0x12,0x11,0x10,0x09,0x08,0x07,0x06,0x05,0x04,0x03},dataread[10]={0},i,j;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
#define  FLASH_WRITE_ADDRESS      0x70000
//#define  FLASH_WRITE_ADDRESS_test      0x60000
#define  FLASH_READ_ADDRESS       FLASH_WRITE_ADDRESS
//#define  FLASH_READ_ADDRESS_test      FLASH_WRITE_ADDRESS_test
#define  FLASH_SECTOR_TO_ERASE    FLASH_WRITE_ADDRESS
//#define  FLASH_SECTOR_TO_ERASE_test   FLASH_WRITE_ADDRESS_test

#define  BufferSize (countof(Tx_Buffer)-1)
//#define  BufferSize_test (countof(Tx_Buffer1)-1)

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
//uint8_t Tx_Buffer[] = "STM32F4xx SPI Firmware Library Example: communication with an M25P SPI FLASH";
uint8_t Tx_Buffer[] ={0xfb,0xfc};
uint8_t  Rx_Buffer[BufferSize];
//uint8_t Rx_Buffer1[]={0,0};
__IO uint8_t Index = 0x0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;
__IO uint32_t FlashID = 0xfa;

/* Private functions ---------------------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
int main(void)
{    
	
  uint16_t s_r,a,b;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOD Periph clock enable */
 // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);//chip select low

  /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  //GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
	
	
         HardwareSPI_init();
	
	//sFLASH_Init();
	
// 	sFLASH_EraseSector(FLASH_SECTOR_TO_ERASE);
// 	sFLASH_EraseSector(FLASH_SECTOR_TO_ERASE_test);
// 	/* Get SPI Flash ID */
  FlashID = sFLASH_ReadID();
//GPIO_SetBits(GPIOD, GPIO_Pin_13);
    /* Write Tx_Buffer data to SPI FLASH memory */
if(  FlashID ==0x00)
{
				GPIO_SetBits(GPIOD, GPIO_Pin_12);
				GPIO_ResetBits(GPIOB, GPIO_Pin_12);//chip select low
				spiPutWord(0x0006);//write eneble
				GPIO_SetBits(GPIOB, GPIO_Pin_12);//chip select high
				GPIO_ResetBits(GPIOB, GPIO_Pin_12);//chip select low
				spiPutWord(0x0001);//write status register
				spiPutWord(0x0000);//value given to ststus register
				GPIO_SetBits(GPIOB, GPIO_Pin_12);//chip select high
				
				GPIO_ResetBits(GPIOB, GPIO_Pin_12);//chip select low
				spiPutWord(0x00c7);//bulk erase
				GPIO_SetBits(GPIOB, GPIO_Pin_12);//chip select high
				GPIO_SetBits(GPIOD, GPIO_Pin_13);
				while(1)
				{
				  GPIO_ResetBits(GPIOB, GPIO_Pin_12);//chip select low
				  spiPutWord(0x0005);//read status register
				  GPIO_SetBits(GPIOB, GPIO_Pin_12);//chip select high
				  GPIO_ResetBits(GPIOB, GPIO_Pin_12);//chip select low
				  s_r=spiGetWord();
			    if( (s_r & 0x0001) ==0)
				  {
					  break;
				  }
			  }
				GPIO_ResetBits(GPIOB, GPIO_Pin_12);//chip select low
				spiPutWord(0x0002);
				spiPutWord(0x0007);
				spiPutWord(0x0000);
				spiPutWord(0x1234);
				spiPutWord(0x2345);
				GPIO_SetBits(GPIOB, GPIO_Pin_12);//chip select high		
				
				while(1)
				{
				  GPIO_ResetBits(GPIOB, GPIO_Pin_12);//chip select low
				  spiPutWord(0x0005);//read status register
				  GPIO_SetBits(GPIOB, GPIO_Pin_12);//chip select high
				  GPIO_ResetBits(GPIOB, GPIO_Pin_12);//chip select low
				  s_r=spiGetWord();
			    if( (s_r & 0x0001) ==0)
				  {
					  break;
				  }
			  }
				GPIO_SetBits(GPIOD, GPIO_Pin_14);
				GPIO_ResetBits(GPIOB, GPIO_Pin_12);//chip select low
				spiPutWord(0x0003);
				spiPutWord(0x0007);
				spiPutWord(0x0000);
				a=spiGetWord();
				b=spiGetWord();
				GPIO_SetBits(GPIOB, GPIO_Pin_12);//chip select high
				GPIO_SetBits(GPIOD, GPIO_Pin_14);
				if(a==0x1234 && b==0x2345)
				{
						GPIO_ResetBits(GPIOD, GPIO_Pin_12);
				}
				else
				{
						GPIO_ResetBits(GPIOD, GPIO_Pin_12);
						GPIO_ResetBits(GPIOD, GPIO_Pin_13);
				}	
			}					
	
	
}  /* TransferStatus1 = PASSED, if the transmitted and received data by SPI1
       are the same */
    /* TransferStatus1 = FAILED, if the transmitted and received data by SPI1
       are different */

    /* Perform an erase in the Flash followed by a read of the written data */
    /* Erase SPI FLASH Sector to write on */
   // sFLASH_EraseSector(FLASH_SECTOR_TO_ERASE);

    /* Read data from SPI FLASH memory */
   // sFLASH_ReadBuffer(Rx_Buffer, FLASH_READ_ADDRESS, BufferSize);

    /* Check the correctness of erasing operation dada */
//     for (Index = 0; Index < BufferSize; Index++)
//     {
//       if (Rx_Buffer[Index] != 0xFF)
//       {
//         TransferStatus2 = FAILED;
// 				GPIO_SetBits(GPIOD, GPIO_Pin_14);
//       }
// 			else
// 			{
// 				/* Error: Turn on LD2 */
// 				GPIO_SetBits(GPIOD, GPIO_Pin_15);
// 				//STM_EVAL_LEDOn(LED2);
// 			}
//     }
    /* TransferStatus2 = PASSED, if the specified sector part is erased */
    /* TransferStatus2 = FAILED, if the specified sector part is not well erased */

  
  
  

//   /* Get SPI Flash ID */
//   FlashID = sFLASH_ReadID();
//   
//   /* Check the SPI Flash ID */
//   if (FlashID == sFLASH_M25P64_ID)
// 	{
// 		
// 			   
// 				 }

//			 }
	void HardwareSPI_init(void) 
{
		
  uint8_t spiConfigured;
  SPI_InitTypeDef SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  // enable the SPI peripheral clock
  SPI_PORT_CLOCK_INIT(SPI_PORT_CLOCK, ENABLE);
  // enable the peripheral GPIO port clocks
  RCC_AHB1PeriphClockCmd(SPI_SCK_GPIO_CLK | SPI_MOSI_GPIO_CLK|SPI_MISO_GPIO_CLK , ENABLE);
  // Connect SPI pins to AF5 - see section 3, Table 6 in the device datasheet
	//GPIO_PinAFConfig(SPI_NSS_GPIO_PORT, SPI_NSS_SOURCE, SPI_NSS_AF);
  GPIO_PinAFConfig(SPI_SCK_GPIO_PORT, SPI_SCK_SOURCE, SPI_SCK_AF);
  GPIO_PinAFConfig(SPI_MOSI_GPIO_PORT, SPI_MOSI_SOURCE, SPI_MOSI_AF);
	GPIO_PinAFConfig(SPI_MISO_GPIO_PORT, SPI_MISO_SOURCE, SPI_MISO_AF);
  // now configure the pins themselves
  // they are all going to be fast push-pull outputs
  // but the SPI pins use the alternate function
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
//	GPIO_InitStructure.GPIO_Pin = SPI_NSS_PIN;
  //GPIO_Init(SPI_NSS_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
  GPIO_Init(SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
  GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
  GPIO_Init(SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
  // now we can set up the SPI peripheral
  // Assume the target is write only and we look after the chip select ourselves
  // SPI clock rate will be system frequency/4/prescaler
  // so here we will go for 72/4/8 = 2.25MHz
  SPI_I2S_DeInit(SPI_PORT);
  SPI_StructInit(&SPI_InitStructure);
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_Direction =SPI_Direction_2Lines_FullDuplex ;
	
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_Init(SPI_PORT, &SPI_InitStructure);
  // Enable the SPI port
  SPI_Cmd(SPI_PORT, ENABLE);
  spiConfigured = 1;
}


void spiPutWord(uint16_t data)
{
  // make sure the transmit buffer is free
  while (SPI_I2S_GetFlagStatus(SPI_PORT, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPI2, data);
 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
};

uint16_t spiGetWord(void) 
{ STM_EVAL_LEDOn(LED6);
		Delay(0xffff);
  // make sure the transmit buffer is free
  while (SPI_I2S_GetFlagStatus(SPI_PORT, SPI_I2S_FLAG_RXNE) == RESET);
  STM_EVAL_LEDOff(LED6);
 
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET);
	STM_EVAL_LEDOff(LED5);
	return SPI_I2S_ReceiveData(SPI2);
};
/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}
/*
#ifdef  USE_FULL_ASSERT

**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  *
void assert_failed(uint8_t* file, uint32_t line)
{ 
  * User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) *

  * Infinite loop *
  while (1)
  {
  }
}
#endif */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
