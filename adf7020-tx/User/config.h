#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include "stm32f4xx.h"

/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define Open_USART                        USART2
/*#define Open_USART_CLK                    RCC_APB1Periph_USART3

#define Open_USART_TX_PIN                 GPIO_Pin_10
#define Open_USART_TX_GPIO_PORT           GPIOC
#define Open_USART_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define Open_USART_TX_SOURCE              GPIO_PinSource10
#define Open_USART_TX_AF                  GPIO_AF_USART3


#define Open_USART_RX_PIN                 GPIO_Pin_11
#define Open_USART_RX_GPIO_PORT           GPIOC
#define Open_USART_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define Open_USART_RX_SOURCE              GPIO_PinSource11
#define Open_USART_RX_AF                  GPIO_AF_USART3*/

#define Open_USART_IRQn          USART2_IRQn         //USART2_IRQn

/**
 * @brief Definition for connected to I2C1
 */
/* SPIx Communication boards Interface */
// SPI1 MISO (PA6)	MOSI(PA7)  CLK(PA5)  NSS(PA4)

#define Open_RCC_APB2Periph_SPIx   	       RCC_APB2Periph_SPI1

#define Open_SPIx                           SPI1
#define Open_SPIx_CLK                       RCC_APB2Periph_SPI1
#define Open_SPIx_CLK_INIT                  RCC_APB1PeriphClockCmd
#define Open_SPIx_IRQn                      SPI1_IRQn
#define Open_SPIx_IRQHANDLER                SPI1_IRQHandler

#define Open_SPIx_SCK_PIN                   GPIO_Pin_5
#define Open_SPIx_SCK_GPIO_PORT             GPIOA
#define Open_SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define Open_SPIx_SCK_SOURCE                GPIO_PinSource5
#define Open_SPIx_SCK_AF                    GPIO_AF_SPI1

#define Open_SPIx_MISO_PIN                  GPIO_Pin_6
#define Open_SPIx_MISO_GPIO_PORT            GPIOA
#define Open_SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define Open_SPIx_MISO_SOURCE               GPIO_PinSource6
#define Open_SPIx_MISO_AF                   GPIO_AF_SPI1

#define Open_SPIx_MOSI_PIN                  GPIO_Pin_7
#define Open_SPIx_MOSI_GPIO_PORT            GPIOA
#define Open_SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define Open_SPIx_MOSI_SOURCE               GPIO_PinSource7
#define Open_SPIx_MOSI_AF                   GPIO_AF_SPI1

#define Open_SPIx_CS_PIN                  GPIO_Pin_4
#define Open_SPIx_CS_GPIO_PORT            GPIOA
#define Open_SPIx_CS_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define Open_SPIx_CS_SOURCE               GPIO_PinSource4
#define Open_SPIx_CS_AF                   GPIO_AF_SPI1


#define Open_RCC_APB2Periph_SPIy   	       RCC_APB1Periph_SPI2

#define Open_SPIy                           SPI2
#define Open_SPIy_CLK                       RCC_APB1Periph_SPI2
#define Open_SPIy_CLK_INIT                  RCC_APB1PeriphClockCmd
#define Open_SPIy_IRQn                      SPI2_IRQn
#define Open_SPIy_IRQHANDLER                SPI2_IRQHandler

#define Open_SPIy_SCK_PIN                   GPIO_Pin_10
#define Open_SPIy_SCK_GPIO_PORT             GPIOB
#define Open_SPIy_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define Open_SPIy_SCK_SOURCE                GPIO_PinSource10
#define Open_SPIy_SCK_AF                    GPIO_AF_SPI1

#define Open_SPIy_MISO_PIN                  GPIO_Pin_2
#define Open_SPIy_MISO_GPIO_PORT            GPIOC
#define Open_SPIy_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define Open_SPIy_MISO_SOURCE               GPIO_PinSource2
#define Open_SPIy_MISO_AF                   GPIO_AF_SPI2

#define Open_SPIy_MOSI_PIN                  GPIO_Pin_3
#define Open_SPIy_MOSI_GPIO_PORT            GPIOC
#define Open_SPIy_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define Open_SPIy_MOSI_SOURCE               GPIO_PinSource3
#define Open_SPIy_MOSI_AF                   GPIO_AF_SPI2

#define Open_SPIy_CS_PIN                  GPIO_Pin_12
#define Open_SPIy_CS_GPIO_PORT            GPIOB
#define Open_SPIy_CS_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define Open_SPIy_CS_SOURCE               GPIO_PinSource12
#define Open_SPIy_CS_AF                   GPIO_AF_SPI2

#endif
