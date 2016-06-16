#include "led.h"

void initial_led1(void)
{
	GPIO_InitTypeDef gpio_initTypeDef;
	
	RCC_AHB1PeriphClockCmd(LED1_CLK,ENABLE);
	
	gpio_initTypeDef.GPIO_Mode = GPIO_Mode_OUT;
	gpio_initTypeDef.GPIO_OType = GPIO_OType_PP;
	gpio_initTypeDef.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_initTypeDef.GPIO_Speed = GPIO_Speed_2MHz;
	gpio_initTypeDef.GPIO_Pin = LED1_R_POART_PIN |LED1_G_POART_PIN |LED1_B_POART_PIN;
	GPIO_Init(LED1_POART,&gpio_initTypeDef);
	
	
	
}
void set_led1(uint16_t GPIO_Pin,LED1_STA newSta)
{
	switch(newSta)
	{
		case LED1_ON:
			GPIO_ResetBits(LED1_POART,GPIO_Pin);
			break;
		case LED1_OFF:
			GPIO_SetBits(LED1_POART,GPIO_Pin);
		break;
	}

}
	

