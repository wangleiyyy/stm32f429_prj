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
void KEY1_NVIC_Config(void)
{
	NVIC_InitTypeDef nvic_itdf;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvic_itdf.NVIC_IRQChannel = KEY1_EXTI_IRQn;
	nvic_itdf.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_itdf.NVIC_IRQChannelSubPriority = 1;
	nvic_itdf.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_itdf);
}
void KEY1_EXTI_Config(void)
{
	EXTI_InitTypeDef exti_itdf;
	initial_key(KEY1_POART,KEY1_PIN,KEY1_CLK);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	KEY1_NVIC_Config();
	SYSCFG_EXTILineConfig(KEY1_EXTI_CFG_POARTSOURCE,KEY1_EXTI_CFG_PINSOURCE);
	
	exti_itdf.EXTI_Line = KEY1_EXTI_LINE;
	exti_itdf.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_itdf.EXTI_Trigger = EXTI_Trigger_Rising;
	exti_itdf.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti_itdf);
}
void KEY2_NVIC_Config(void)
{
	NVIC_InitTypeDef nvic_itdf;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	nvic_itdf.NVIC_IRQChannel = KEY2_EXTI_IRQn;
	nvic_itdf.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_itdf.NVIC_IRQChannelSubPriority = 1;
	nvic_itdf.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_itdf);
}
void KEY2_EXTI_Config(void)
{
	EXTI_InitTypeDef exti_itdf;
	initial_key(KEY2_POART,KEY2_PIN,KEY2_CLK);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	KEY2_NVIC_Config();
	SYSCFG_EXTILineConfig(KEY2_EXTI_CFG_POARTSOURCE,KEY2_EXTI_CFG_PINSOURCE);
	
	exti_itdf.EXTI_Line = KEY2_EXTI_LINE;
	exti_itdf.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_itdf.EXTI_Trigger = EXTI_Trigger_Rising;
	exti_itdf.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti_itdf);
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
