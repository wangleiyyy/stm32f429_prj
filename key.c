#include "key.h"

void initial_key(GPIO_TypeDef* key_port, uint16_t key_pin,uint32_t key_clk)
{
	GPIO_InitTypeDef gpio_initial;
	RCC_AHB1PeriphClockCmd(key_clk, ENABLE);
	gpio_initial.GPIO_Mode = GPIO_Mode_IN;
	gpio_initial.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_initial.GPIO_Pin = key_pin;
	GPIO_Init(key_port,&gpio_initial);
}
	
uint8_t scan_key_state(GPIO_TypeDef* key_port,uint16_t key_pin)
{
	if(GPIO_ReadInputDataBit(key_port,key_pin) == KEY_ON)
	{
		return KEY_ON;
	}
	else
	{
		return KEY_OFF;
	}
}
