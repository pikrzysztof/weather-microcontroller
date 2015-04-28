#include "usart.h"
#include <stm32.h>
#include <gpio.h>
#include <stdint.h>

#define GPIO_AF_USART2 7
#define USART_CR1_8B 0x0000
#define USART_CR1_PARITY_NO 0x0000
#define USART_CR2_StopBits_1 0x0000
#define USART_CR3_NONE 0x0000
#define USART_WordLength_8b 0x0000
#define USART_Mode_Rx_Tx (USART_CR1_RE | USART_CR1_TE)
#define USART_Parity_No 0x0000
#define USART_StopBits_1 0x0000
#define USART_FlowControl_None 0x0000
#define HSI_VALUE 16000000
#define USART_Enable USART_CR1_UE

void usart_ustaw()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	GPIOafConfigure(GPIOA,
			2U,
			GPIO_OType_PP,
			GPIO_Fast_Speed,
			GPIO_PuPd_NOPULL,
			GPIO_AF_USART2);
	GPIOafConfigure(GPIOA,
			3U,
			GPIO_OType_PP,
			GPIO_Fast_Speed,
			GPIO_PuPd_UP,
			GPIO_AF_USART2);
	USART2->CR1 = USART_Mode_Rx_Tx | USART_WordLength_8b |
		USART_Parity_No;
	USART2->CR2 = USART_StopBits_1;
	USART2->CR3 = USART_FlowControl_None;
	unsigned baudrate = 9600;
	USART2->BRR = (HSI_VALUE + (baudrate / 2U)) /
		baudrate;
	USART2->CR1 |= USART_Enable;

}

static void nadaj_znak(char znak)
{
	while (!(USART2->SR & USART_SR_TXE)) {
		;
	}
	USART2->DR = znak;
}

void usart_nadajbeznl(char *tekst)
{
	int i = 0;
	while (i[tekst] != '\0') {
		nadaj_znak(i[tekst]);
		++i;
	}
}

void usart_nadaj(char *tekst)
{
	usart_nadajbeznl(tekst);
	nadaj_znak('\r');
	nadaj_znak('\n');
}

void usart_nadaj_liczbe(uint8_t liczba)
{
	char bufor[4];
	int i = 0;
	if (liczba == 0) {
		usart_nadajbeznl("0");
	} else {
		while (liczba != 0) {
			bufor[i++] = (char) ((liczba % 10) + 48);
			liczba /= 10;
		}
		for (i--; i >= 0; i--) {
			nadaj_znak(bufor[i]);
		}
	}
}
