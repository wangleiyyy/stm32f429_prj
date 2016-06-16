#include "stm32f4xx.h"
#include "led.h"

int main(void)
{
	int i = 0;
	initial_led1();


	while(1)
	{
		
		
		
		
		for(i=0;i<10000000;i++);
		set_led1(LED1_R_POART_PIN,LED1_ON);
		set_led1(LED1_G_POART_PIN|LED1_B_POART_PIN,LED1_OFF);
		for(i=0;i<10000000;i++);
		set_led1(LED1_G_POART_PIN,LED1_ON);
		set_led1(LED1_R_POART_PIN|LED1_B_POART_PIN,LED1_OFF);
		for(i=0;i<10000000;i++);
		set_led1(LED1_B_POART_PIN,LED1_ON);
		set_led1(LED1_R_POART_PIN|LED1_G_POART_PIN,LED1_OFF);
		for(i=0;i<10000000;i++);
		set_led1(LED1_R_POART_PIN|LED1_G_POART_PIN|LED1_B_POART_PIN,LED1_ON);


	}
}
