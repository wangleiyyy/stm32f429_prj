#include "usb_uart.h"

static void NVIC_USB_USART232_config(void)
{
	NVIC_InitTypeDef nvic_initTypeDef;
	nvic_initTypeDef.NVIC_IRQChannel = USB_USART232_IRQ;
	nvic_initTypeDef.NVIC_IRQChannelCmd = ENABLE;
	nvic_initTypeDef.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_initTypeDef.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&nvic_initTypeDef);
}
void USB_USART232_Initial(void)
{
	GPIO_InitTypeDef gpio_initTypeDef;
	USART_InitTypeDef usart_initTypeDef;
	
	RCC_AHB1PeriphClockCmd(USB_USART232_Rx_RCC | USB_USART232_Tx_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(USB_USART232_CLK,ENABLE);
	
	/*GPIO initial*/
	gpio_initTypeDef.GPIO_Mode = GPIO_Mode_AF;
	gpio_initTypeDef.GPIO_OType = GPIO_OType_PP;
	gpio_initTypeDef.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_initTypeDef.GPIO_Speed = GPIO_Speed_50MHz;
	
	gpio_initTypeDef.GPIO_Pin = USB_USART232_Rx_PIN;
	GPIO_Init(USB_USART232_Rx_PORT,&gpio_initTypeDef);
	gpio_initTypeDef.GPIO_Pin = USB_USART232_Tx_PIN;
	GPIO_Init(USB_USART232_Tx_PORT,&gpio_initTypeDef);

	GPIO_PinAFConfig(USB_USART232_Rx_PORT,USB_USART232_Rx_SOURCE,USB_USART232_Rx_AF);
	GPIO_PinAFConfig(USB_USART232_Tx_PORT,USB_USART232_Tx_SOURCE,USB_USART232_Tx_AF);
	/*usb_usart232 initial*/
	usart_initTypeDef.USART_BaudRate = USB_USART232_BAUDRATE;
	usart_initTypeDef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_initTypeDef.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart_initTypeDef.USART_Parity = USART_Parity_No;
	usart_initTypeDef.USART_StopBits = USART_StopBits_1;
	usart_initTypeDef.USART_WordLength = USART_WordLength_8b;
	USART_Init(USB_USART232,&usart_initTypeDef);
	
	NVIC_USB_USART232_config();
	
	USART_ITConfig(USB_USART232,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USB_USART232,ENABLE);
	
	
}

/*****************  ����һ���ַ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USB_USART232, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USB_USART232, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USB_USART232, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USB_USART232);
}
