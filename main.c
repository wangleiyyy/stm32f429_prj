#include "stm32f4xx.h"
#include "led.h"

int main(void)
{
	initial_led1();
	set_led1(LED1_R_POART_PIN,LED1_OFF);

	while(1);
}
