#ifndef BH_BOARD_E2PROM
#define BH_BOARD_E2PROM
#include "stm32f4xx.h"

#define E2PROM_I2C									I2C1
#define E2PROM_I2C_CLK 							RCC_APB1Periph_I2C1
#define E2PROM_I2C_SPEED						400000
#define E2PROM_I2C_OWN_ADDR					0x0A

/*GPIO define*/
#define E2PROM_I2C_SCL_PORT					GPIOB
#define E2PROM_I2C_SCL_PIN					GPIO_Pin_6
#define E2PROM_I2C_SCL_CLK					RCC_AHB1Periph_GPIOB
#define E2PROM_I2C_SCL_PIN_SOURCE 	GPIO_PinSource6
#define E2PROM_I2C_SCL_AF						GPIO_AF_I2C1


#define E2PROM_I2C_SDA_PORT					GPIOB
#define E2PROM_I2C_SDA_PIN					GPIO_Pin_7
#define E2PROM_I2C_SDA_CLK					RCC_AHB1Periph_GPIOB
#define E2PROM_I2C_SDA_PIN_SOURCE 	GPIO_PinSource7
#define E2PROM_I2C_SDA_AF						GPIO_AF_I2C1

/*wait time for the timeout */
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

#define E2PROM_ADDR					0xA0
/* AT24C01/02每页有8个字节 */
#define E2PROM_PageSize           8


void e2prom_initial(void);
uint32_t E2PROM_ByteWrite(u8* pBuffer, u8 WriteAddr);
void E2PROM_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite);
uint32_t E2PROM_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite);	
uint32_t E2PROM_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);




#endif

