#ifndef _BH_BOARD_PERIPH_KEY_H
#define _BH_BOARD_PERIPH_KEY_H
#include "stm32f4xx.h"

#define KEY_ON 1
#define KEY_OFF 0

#define KEY1_POART 								GPIOA
#define KEY1_PIN 									GPIO_Pin_0
#define KEY1_CLK 									RCC_AHB1Periph_GPIOA

#define KEY1_EXTI_CFG_POARTSOURCE EXTI_PortSourceGPIOA
#define KEY1_EXTI_CFG_PINSOURCE		EXTI_PinSource0	 

#define KEY1_EXTI_IRQn 						EXTI0_IRQn
#define KEY1_EXTI_LINE						EXTI_Line0

#define KEY1_EXTI_IRQHandler 			EXTI0_IRQHandler



#define KEY2_POART 								GPIOC
#define KEY2_PIN 									GPIO_Pin_13
#define KEY2_CLK 									RCC_AHB1Periph_GPIOC

#define KEY2_EXTI_CFG_POARTSOURCE EXTI_PortSourceGPIOC
#define KEY2_EXTI_CFG_PINSOURCE		EXTI_PinSource13

#define KEY2_EXTI_IRQn 						EXTI15_10_IRQn
#define KEY2_EXTI_LINE						EXTI_Line13

#define KEY2_EXTI_IRQHandler 			EXTI15_10_IRQHandler


void initial_key(GPIO_TypeDef* key_port,uint16_t key_pin,uint32_t key_clk);
uint8_t scan_key_state(GPIO_TypeDef* key_port,uint16_t key_pin);
void KEY1_NVIC_Config(void);
void KEY2_NVIC_Config(void);
void KEY1_EXTI_Config(void);
void KEY2_EXTI_Config(void);



#endif

