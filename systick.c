#define SYSTEM_TICK_GLOBAL
#include "systick.h"


void SysTickInitial(void)
{
	ss = 0;
	mm = 0;
	hh = 0;
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if(SysTick_Config(SystemCoreClock/100000))
	{
		while(1);
	}
	
}
void SysTick_it(void)
{
	static __IO uint32_t systick_it= 0;

	systick_it++;
	if(systick_it % 100 == 0)
	{
		SysTick_1ms();
	}
	if(systick_it %100000== 0)
	{
		SysTick_1s();
	}

 	
	
}

void SysTick_1ms(void)
{
	
}

void SysTick_1s(void)
{
	ss++;
	if(ss%2 == 0)
	{
		set_led1(LED1_R_POART_PIN,LED1_ON);
		set_led1(LED1_G_POART_PIN|LED1_B_POART_PIN,LED1_OFF);
	}
	else
	{
		set_led1(LED1_G_POART_PIN,LED1_ON);
		set_led1(LED1_R_POART_PIN|LED1_B_POART_PIN,LED1_OFF);
	}
	
	
	
}

