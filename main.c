#include "stm32f4xx.h"
#include "led.h"
#include "key.h"

int main(void)
{
	int i = 0;
	initial_led1();
	initial_key(KEY1_POART,KEY1_PIN,KEY1_CLK);
	initial_key(KEY2_POART,KEY2_PIN,KEY2_CLK);
	while(1)
	{
		if(scan_key_state(KEY1_POART,KEY1_PIN) == KEY_ON)
		{
				set_led1(LED1_R_POART_PIN,LED1_ON);
				set_led1(LED1_G_POART_PIN|LED1_B_POART_PIN,LED1_OFF);
		}
		else
		{
				set_led1(LED1_R_POART_PIN,LED1_OFF);
		}
		if(scan_key_state(KEY2_POART,KEY2_PIN) == KEY_ON)
		{
				set_led1(LED1_G_POART_PIN,LED1_ON);
				set_led1(LED1_R_POART_PIN|LED1_B_POART_PIN,LED1_OFF);
		}
		else
		{
				set_led1(LED1_G_POART_PIN,LED1_OFF);
		}
		
//		for(i=0;i<10000000;i++);
//		set_led1(LED1_R_POART_PIN,LED1_ON);
//		set_led1(LED1_G_POART_PIN|LED1_B_POART_PIN,LED1_OFF);
//		for(i=0;i<10000000;i++);
//		set_led1(LED1_G_POART_PIN,LED1_ON);
//		set_led1(LED1_R_POART_PIN|LED1_B_POART_PIN,LED1_OFF);
//		for(i=0;i<10000000;i++);
//		set_led1(LED1_B_POART_PIN,LED1_ON);
//		set_led1(LED1_R_POART_PIN|LED1_G_POART_PIN,LED1_OFF);
//		for(i=0;i<10000000;i++);
//		set_led1(LED1_R_POART_PIN|LED1_G_POART_PIN|LED1_B_POART_PIN,LED1_ON);
	}
}
