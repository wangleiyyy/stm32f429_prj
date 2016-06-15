#ifndef BH_BOARD_PERIPH_LED_H
#define BH_BOARD_PERIPH_LED_H
#include "stm32f4xx.h"

#define LED1_POART 			  GPIOH
#define LED1_CLK  				RCC_AHB1Periph_GPIOH
#define LED1_R_POART_PIN  GPIO_Pin_10
#define LED1_G_POART_PIN  GPIO_Pin_11
#define LED1_B_POART_PIN  GPIO_Pin_12


typedef enum {LED1_ON = 0, LED1_OFF = !LED1_ON} LED1_STA;


void initial_led1(void);
void set_led1(uint16_t GPIO_Pin,LED1_STA newSta);

#endif

