#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

#define SPI2_CS PDout(8)	

#define SPI                             SPI2
#define SPI_TIMER                       RCC_APB1Periph_SPI2
#define SPI_PORT_TIMER                  RCC_AHB1Periph_GPIOB
#define SPI_PORT                        GPIOB
#define SPI_PORT_GPIO_PinSource_SCL     GPIO_PinSource13
#define SPI_PORT_GPIO_PinSource_MISO    GPIO_PinSource14
#define SPI_PORT_GPIO_PinSource_MOSI    GPIO_PinSource15
#define SPI_PORT_GPIO_AF                GPIO_AF_SPI2
#define SPI_PORT_PIN                    GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15

void SPI2_Init(void);			          //³õÊ¼»¯SPI2¿Ú  
void SPI2_WriteByte(u8 TxData);
void SPI2_Write(u8 TxData1 ,u16 TxData2);

u8 SPI2_ReadByte(u8 TxData);
u16 SPI2_Read(u8 TxData);
#endif

