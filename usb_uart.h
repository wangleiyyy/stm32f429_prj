#ifndef BH_BOARD_USB_UART
#define BH_BOARD_USB_UART
#include "stm32f4xx.h"
#include "stdio.h"

#define USB_USART232 								USART1
#define USB_USART232_CLK 						RCC_APB2Periph_USART1
#define USB_USART232_BAUDRATE				115200

#define USB_USART232_Tx_PORT				GPIOA
#define USB_USART232_Tx_RCC					RCC_APB2Periph_USART1
#define USB_USART232_Tx_PIN					GPIO_Pin_9
#define USB_USART232_Tx_AF					GPIO_AF_USART1
#define USB_USART232_Tx_SOURCE			GPIO_PinSource9

#define USB_USART232_Rx_PORT				GPIOA
#define USB_USART232_Rx_RCC					RCC_APB2Periph_USART1
#define USB_USART232_Rx_PIN					GPIO_Pin_10
#define USB_USART232_Rx_AF					GPIO_AF_USART1
#define USB_USART232_Rx_SOURCE			GPIO_PinSource10

#define USB_USART232_IRQHandler			USART1_IRQHandler
#define USB_USART232_IRQ						USART1_IRQn


void USB_USART232_Initial(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);


#endif

