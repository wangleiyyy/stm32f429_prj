#ifndef BH_SYS_TICK
#define BH_SYS_TICK
#include "stm32f4xx.h"
#include "led.h"

#ifndef SYSTEM_TICK_GLOBAL
#define SYSTEM_TICK_GLOBAL extern
#else 
#define SYSTEM_TICK_GLOBAL
#endif

SYSTEM_TICK_GLOBAL uint32_t ss;
SYSTEM_TICK_GLOBAL uint32_t mm;
SYSTEM_TICK_GLOBAL uint32_t hh;




void SysTickInitial(void);
void SysTick_it(void);
void SysTick_1ms(void);
void SysTick_1s(void);





#endif

