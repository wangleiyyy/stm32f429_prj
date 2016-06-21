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

	
	 printf("\r\nָ�뷽ʽ����SDRAM\r\n");
	/*��SDRAMд��8λ����*/
	 *( uint8_t*) (SDRAM_BANK_ADDR ) = (uint8_t)0xAA;
	 printf("\r\nָ�����SDRAM��д������0xAA \r\n");

	 /*��SDRAM��ȡ����*/
	 temp =  *( uint8_t*) (SDRAM_BANK_ADDR );
	 printf("��ȡ���ݣ�0x%X \r\n",temp);

	 /*д/�� 16λ����*/
	 *( uint16_t*) (SDRAM_BANK_ADDR+10 ) = (uint16_t)0xBBBB;
	 printf("ָ�����SDRAM��д������0xBBBB \r\n");
	 
	 temp =  *( uint16_t*) (SDRAM_BANK_ADDR+10 );
	 printf("��ȡ���ݣ�0x%X \r\n",temp);


	 /*д/�� 32λ����*/
	 *( uint32_t*) (SDRAM_BANK_ADDR+20 ) = (uint32_t)0xCCCCCCCC;
	 printf("ָ�����SDRAM��д������0xCCCCCCCC \r\n");	 
	 temp =  *( uint32_t*) (SDRAM_BANK_ADDR+20 );
	 printf("��ȡ���ݣ�0x%X \r\n",temp);
	
	while(1)
	{
		
	}
}
/**
  * @brief  I2C(AT24C02)��д����
  * @param  ��
  * @retval ��������1 ������������0
  */
uint8_t I2C_Test(void)
{
	u16 i;
	uint32_t res = 0 ;
	#define  EEP_Firstpage      0x00
	uint8_t I2c_Buf_Write[256];
	uint8_t I2c_Buf_Read[256];
    
	for ( i=0; i<=255; i++ ) //��仺��
  {   
    I2c_Buf_Write[i] = i;   
   }
	E2PROM_ByteWrite(I2c_Buf_Write, EEP_Firstpage);

  //��I2c_Buf_Write��˳�����������д��EERPOM�� 
	E2PROM_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 256);
  
 //��EEPROM��������˳�򱣳ֵ�I2c_Buf_Read��
	E2PROM_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256); 
   
  //��I2c_Buf_Read�е�����ͨ�����ڴ�ӡ
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

