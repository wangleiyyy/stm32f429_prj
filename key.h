#ifndef _BH_BOARD_PERIPH_KEY_H
#define _BH_BOARD_PERIPH_KEY_H
#include "stm32f4xx.h"

#define KEY_ON 1
#define KEY_OFF 0

#define KEY1_POART GPIOA
#define KEY1_PIN GPIO_Pin_0
#define KEY1_CLK RCC_AHB1Periph_GPIOA

#define KEY2_POART GPIOC
#define KEY2_PIN GPIO_Pin_13
#define KEY2_CLK RCC_AHB1Periph_GPIOC

void initial_key(GPIO_TypeDef* key_port,uint16_t key_pin,uint32_t key_clk);
uint8_t scan_key_state(GPIO_TypeDef* key_port,uint16_t key_pin);


#endif

