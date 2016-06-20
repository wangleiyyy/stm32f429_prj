#include "e2prom.h"



static void gpio_initial(void);
static void i2c_initial(void);
static void I2C_EE_WaitEepromStandbyState(void);
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);

uint32_t I2CTimeout = 0;

//Public 

void e2prom_initial(void)
{
	gpio_initial();
	i2c_initial();
}
/**
  * @brief   写一个字节到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  无
  */
uint32_t E2PROM_ByteWrite(u8* pBuffer, u8 WriteAddr)
{
  /* Send STRAT condition */
  I2C_GenerateSTART(E2PROM_I2C, ENABLE);

  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  }    

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(E2PROM_I2C, E2PROM_ADDR, I2C_Direction_Transmitter);
  
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }    
      
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(E2PROM_I2C, WriteAddr);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  } 
  /* Send the byte to be written */
  I2C_SendData(E2PROM_I2C, *pBuffer); 
   
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  } 
  
  /* Send STOP condition */
  I2C_GenerateSTOP(E2PROM_I2C, ENABLE);
  
  return 1;
}

/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void E2PROM_BufferWrite(u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % E2PROM_PageSize;
  count = E2PROM_PageSize - Addr;
  NumOfPage =  NumByteToWrite / E2PROM_PageSize;
  NumOfSingle = NumByteToWrite % E2PROM_PageSize;
 
  /* If WriteAddr is E2PROM_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < E2PROM_PageSize */
    if(NumOfPage == 0) 
    {
      E2PROM_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > E2PROM_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        E2PROM_PageWrite(pBuffer, WriteAddr, E2PROM_PageSize); 
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  E2PROM_PageSize;
        pBuffer += E2PROM_PageSize;
      }

      if(NumOfSingle!=0)
      {
        E2PROM_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not E2PROM_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < E2PROM_PageSize */
    if(NumOfPage== 0) 
    {
      E2PROM_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > E2PROM_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / E2PROM_PageSize;
      NumOfSingle = NumByteToWrite % E2PROM_PageSize;	
      
      if(count != 0)
      {  
        E2PROM_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        E2PROM_PageWrite(pBuffer, WriteAddr, E2PROM_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  E2PROM_PageSize;
        pBuffer += E2PROM_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        E2PROM_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}
/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  无
  */
uint32_t E2PROM_PageWrite(u8* pBuffer, u8 WriteAddr, u8 NumByteToWrite)
{
  I2CTimeout = I2CT_LONG_TIMEOUT;

  while(I2C_GetFlagStatus(E2PROM_I2C, I2C_FLAG_BUSY))  
   {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(4);
  } 
  
  /* Send START condition */
  I2C_GenerateSTART(E2PROM_I2C, ENABLE);
  
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(5);
  } 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(E2PROM_I2C, E2PROM_ADDR, I2C_Direction_Transmitter);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(6);
  } 
  /* Send the EEPROM's internal address to write to */    
  I2C_SendData(E2PROM_I2C, WriteAddr);  

  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) 
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(7);
  } 
  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(E2PROM_I2C, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(8);
    } 
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(E2PROM_I2C, ENABLE);
  
  return 1;
}

/**
  * @brief   从EEPROM里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg WriteAddr:接收数据的EEPROM的地址
  *     @arg NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */
uint32_t E2PROM_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{  
    I2CTimeout = I2CT_LONG_TIMEOUT;

  //*((u8 *)0x4001080c) |=0x80; 
    while(I2C_GetFlagStatus(E2PROM_I2C, I2C_FLAG_BUSY))   
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
    }
  /* Send START condition */
  I2C_GenerateSTART(E2PROM_I2C, ENABLE);
  //*((u8 *)0x4001080c) &=~0x80;
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
   }

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(E2PROM_I2C, E2PROM_ADDR, I2C_Direction_Transmitter);

  I2CTimeout = I2CT_FLAG_TIMEOUT;
 
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
   }
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(E2PROM_I2C, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(E2PROM_I2C, ReadAddr);  

     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
   }
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(E2PROM_I2C, ENABLE);
  
     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
   }
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(E2PROM_I2C, E2PROM_ADDR, I2C_Direction_Receiver);
  
     I2CTimeout = I2CT_FLAG_TIMEOUT;

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
   }
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(E2PROM_I2C, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(E2PROM_I2C, ENABLE);
    }

		
		I2CTimeout = I2CT_LONG_TIMEOUT;
		while(I2C_CheckEvent(E2PROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
		} 	
		{
		  /* Read a byte from the device */
      *pBuffer = I2C_ReceiveData(E2PROM_I2C);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;
		}			
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(E2PROM_I2C, ENABLE);
  
  return 1;
}

//static

static void gpio_initial(void)
{
	GPIO_InitTypeDef gpio_initTypeDef;
	
	RCC_AHB1PeriphClockCmd(E2PROM_I2C_SCL_CLK | E2PROM_I2C_SDA_CLK,ENABLE);
	GPIO_PinAFConfig(E2PROM_I2C_SCL_PORT,E2PROM_I2C_SCL_PIN_SOURCE, E2PROM_I2C_SCL_AF);
	GPIO_PinAFConfig(E2PROM_I2C_SDA_PORT,E2PROM_I2C_SDA_PIN_SOURCE, E2PROM_I2C_SDA_AF);
	gpio_initTypeDef.GPIO_Mode = GPIO_Mode_AF;
	gpio_initTypeDef.GPIO_OType = GPIO_OType_OD;
	gpio_initTypeDef.GPIO_Pin = E2PROM_I2C_SCL_PIN;
	gpio_initTypeDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_initTypeDef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(E2PROM_I2C_SCL_PORT,&gpio_initTypeDef);
	gpio_initTypeDef.GPIO_Pin = E2PROM_I2C_SDA_PIN;
	GPIO_Init(E2PROM_I2C_SDA_PORT,&gpio_initTypeDef);
}
static void i2c_initial(void)
{
	I2C_InitTypeDef i2c_initTypeDef;
	RCC_APB1PeriphClockCmd(E2PROM_I2C_CLK,ENABLE);
	i2c_initTypeDef.I2C_Ack = I2C_Ack_Enable;
	i2c_initTypeDef.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	i2c_initTypeDef.I2C_ClockSpeed = E2PROM_I2C_SPEED;
	i2c_initTypeDef.I2C_DutyCycle = I2C_DutyCycle_2;
	i2c_initTypeDef.I2C_Mode = I2C_Mode_I2C;
	i2c_initTypeDef.I2C_OwnAddress1 = E2PROM_I2C_OWN_ADDR;
	I2C_Init(E2PROM_I2C,&i2c_initTypeDef);
	I2C_Cmd(E2PROM_I2C,ENABLE);
}
/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示IIC读取失败.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  //EEPROM_ERROR("I2C 等待超时!errorCode = %d",errorCode);
  
  return 0;
}
/**
  * @brief  Wait for EEPROM Standby state 
  * @param  无
  * @retval 无
  */
static void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(E2PROM_I2C, ENABLE);
    /* Read E2PROM_I2C SR1 register */
    SR1_Tmp = I2C_ReadRegister(E2PROM_I2C, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(E2PROM_I2C, E2PROM_ADDR, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(E2PROM_I2C, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(E2PROM_I2C, I2C_FLAG_AF);
  /* STOP condition */    
  I2C_GenerateSTOP(E2PROM_I2C, ENABLE); 
}





