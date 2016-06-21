#include "stm32f4xx.h"
#include "stdio.h"
#include "led.h"
#include "key.h"
#include "systick.h"
#include "usb_uart.h"
#include "e2prom.h"
#include "sdram.h"
uint8_t I2C_Test(void);
int main(void)
{
			 uint32_t temp;
	initial_led1();
//	initial_key(KEY1_POART,KEY1_PIN,KEY1_CLK);
//	initial_key(KEY2_POART,KEY2_PIN,KEY2_CLK);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	KEY1_EXTI_Config();
	KEY2_EXTI_Config();
	USB_USART232_Initial();
	SysTickInitial();
	e2prom_initial();
  SDRAM_Init();
	
	
	Usart_SendByte(USB_USART232,'o');

	if(I2C_Test() ==1)
	{
	
	}
	else
	{

	}
	
	if(SDRAM_Test() ==1)
	{
	
	}
	else
	{

	}

	
	 printf("\r\n指针方式访问SDRAM\r\n");
	/*向SDRAM写入8位数据*/
	 *( uint8_t*) (SDRAM_BANK_ADDR ) = (uint8_t)0xAA;
	 printf("\r\n指针访问SDRAM，写入数据0xAA \r\n");

	 /*从SDRAM读取数据*/
	 temp =  *( uint8_t*) (SDRAM_BANK_ADDR );
	 printf("读取数据：0x%X \r\n",temp);

	 /*写/读 16位数据*/
	 *( uint16_t*) (SDRAM_BANK_ADDR+10 ) = (uint16_t)0xBBBB;
	 printf("指针访问SDRAM，写入数据0xBBBB \r\n");
	 
	 temp =  *( uint16_t*) (SDRAM_BANK_ADDR+10 );
	 printf("读取数据：0x%X \r\n",temp);


	 /*写/读 32位数据*/
	 *( uint32_t*) (SDRAM_BANK_ADDR+20 ) = (uint32_t)0xCCCCCCCC;
	 printf("指针访问SDRAM，写入数据0xCCCCCCCC \r\n");	 
	 temp =  *( uint32_t*) (SDRAM_BANK_ADDR+20 );
	 printf("读取数据：0x%X \r\n",temp);
	
	while(1)
	{
		
	}
}
/**
  * @brief  I2C(AT24C02)读写测试
  * @param  无
  * @retval 正常返回1 ，不正常返回0
  */
uint8_t I2C_Test(void)
{
	u16 i;
	uint32_t res = 0 ;
	#define  EEP_Firstpage      0x00
	uint8_t I2c_Buf_Write[256];
	uint8_t I2c_Buf_Read[256];
    
	for ( i=0; i<=255; i++ ) //填充缓冲
  {   
    I2c_Buf_Write[i] = i;   
   }
	E2PROM_ByteWrite(I2c_Buf_Write, EEP_Firstpage);

  //将I2c_Buf_Write中顺序递增的数据写入EERPOM中 
	E2PROM_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 256);
  
 //将EEPROM读出数据顺序保持到I2c_Buf_Read中
	E2PROM_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256); 
   
  //将I2c_Buf_Read中的数据通过串口打印
	for (i=0; i<256; i++)
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			return 0;
		}
	}
	return 1;
}

/*********************************************END OF FILE**********************/

