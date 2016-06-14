#include "stm32f4xx.h"

/*片上外设基地址  */
#define PERIPH_BASE           ((unsigned int)0x40000000)                          

/*总线基地址 */
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000)	

/*GPIO外设基地址*/
#define GPIOH_BASE            (AHB1PERIPH_BASE + 0x1C00)


/* GPIOH寄存器地址,强制转换成指针 */
#define GPIOH_MODER				*(unsigned int*)(GPIOH_BASE+0x00)
#define GPIOH_OTYPER			*(unsigned int*)(GPIOH_BASE+0x04)
#define GPIOH_OSPEEDR			*(unsigned int*)(GPIOH_BASE+0x08)
#define GPIOH_PUPDR				*(unsigned int*)(GPIOH_BASE+0x0C)
#define GPIOH_IDR					*(unsigned int*)(GPIOH_BASE+0x10)
#define GPIOH_ODR					*(unsigned int*)(GPIOH_BASE+0x14)
#define GPIOH_BSRR					*(unsigned int*)(GPIOH_BASE+0x18)
#define GPIOH_LCKR					*(unsigned int*)(GPIOH_BASE+0x1C)
#define GPIOH_AFRL					*(unsigned int*)(GPIOH_BASE+0x20)
#define GPIOH_AFRH					*(unsigned int*)(GPIOH_BASE+0x24)

/*RCC外设基地址*/
#define RCC_BASE              (AHB1PERIPH_BASE + 0x3800)

/*RCC的AHB1时钟使能寄存器地址,强制转换成指针*/
#define RCC_AHB1ENR				*(unsigned int*)(RCC_BASE+0x30)
	
#define LED_PIN 12

int main(void)
{
/*?? GPIOH ??,??????????????*/
	RCC_AHB1ENR |= (1<<7);	
	
	/* LED ????? */
	
	/*GPIOH MODER10??*/
	GPIOH_MODER  &= ~( 0x03<< (2*LED_PIN));	
	/*PH10 MODER10 = 01b ????*/
	GPIOH_MODER |= (1<<2*LED_PIN);
	
	/*GPIOH OTYPER10??*/
	GPIOH_OTYPER &= ~(1<<1*LED_PIN);
	/*PH10 OTYPER10 = 0b ????*/
	GPIOH_OTYPER |= (0<<1*LED_PIN);
	
	/*GPIOH OSPEEDR10??*/
	GPIOH_OSPEEDR &= ~(0x03<<2*LED_PIN);
	/*PH10 OSPEEDR10 = 0b ??2MHz*/
	GPIOH_OSPEEDR |= (0<<2*LED_PIN);
	
	/*GPIOH PUPDR10??*/
	GPIOH_PUPDR &= ~(0x03<<2*LED_PIN);
	/*PH10 PUPDR10 = 01b ????*/
	GPIOH_PUPDR |= (1<<2*LED_PIN);
	
	/*PH10 BSRR???? BR10?1,????????*/
	GPIOH_BSRR |= (1<<16<<LED_PIN);
	
	/*PH10 BSRR???? BS10?1,????????*/
	//GPIOH_BSRR |= (1<<10);

	while(1);
}
