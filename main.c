#include <stm32.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "zegary.h"
#include "usart.h"
#include "nozki.h"
#include "i2c.h"
#include "diodki.h"
#define HSI_VALUE 16000000U 	/* 16 MHz */
/* #define LISDE_ADDR 001110xb - byc moze, strona 18 http://www.st.com/web/en/resource/technical/document/datasheet/CD00231405.pdf */
#define LIS35DE_ADDR 0x1C
#define LIS35DE_X_VAL_REG 0x29
#define LIS35DE_Y_VAL_REG 0x2B
#define LIS35DE_Z_VAL_REG 0x2D
#define GLOWNY_REJESTR_KONF_LIS35DE 0x20

void wlacz_przerwania(void)
{
	for (size_t i = 0; i < LICZBA_WYPROWADZEN; ++i) {
		if (jest_kukielka(i)) {
			continue;
		}
		NVIC_SetPriority(wy[i].pr.GRUPA_PRZERWAN,
				 wy[i].pr.PIORYTET);
		NVIC_ClearPendingIRQ(wy[i].pr.GRUPA_PRZERWAN);
		NVIC_EnableIRQ(wy[i].pr.GRUPA_PRZERWAN);
	}
}

bool chcemy_debug = false;

void EXTI0_IRQHandler(void)
{
	EXTI->PR = (unsigned int) (-1);
	chcemy_debug = true;
}
void EXTI3_IRQHandler(void)
{
	EXTI->PR = (unsigned int) (-1);
	chcemy_debug = true;
}
void EXTI4_IRQHandler(void)
{
	EXTI->PR = (unsigned int) (-1);
	chcemy_debug = true;
}
void EXTI9_5_IRQHandler(void)
{
	EXTI->PR = (unsigned int) (-1);
	chcemy_debug = true;
}
void EXTI15_10_IRQHandler(void)
{
	EXTI->PR = (unsigned int) (-1);
	chcemy_debug = true;
}

int main()
{
	usart_ustaw();
	usart_nadaj("usart");
	ustaw_i2c(HSI_VALUE);
	usart_nadaj("po i2c");
	ustaw_pwm();
	usart_nadaj("pwm");
	wpisz_do_rejestru(LIS35DE_ADDR, GLOWNY_REJESTR_KONF_LIS35DE, 0x47);
	usart_nadaj("wpisane do rejestru");
	ustaw_guziki();
	wlacz_przerwania();
	for (;;) {
		int8_t x = daj_zawartosc_rejestru(LIS35DE_ADDR,
						   LIS35DE_X_VAL_REG);
		int8_t y = daj_zawartosc_rejestru(LIS35DE_ADDR,
						   LIS35DE_Y_VAL_REG);
		int8_t z = daj_zawartosc_rejestru(LIS35DE_ADDR,
						   LIS35DE_Z_VAL_REG);
		if (x < 0)
			x *= -1;
		if (y < 0)
			y *= -1;
		if (z < 0)
			z *= -1;
		wypelnij(x, y, z);
		if (!chcemy_debug)
			continue;
		chcemy_debug = false;
		usart_nadajbeznl("x ");
		usart_nadaj_liczbe(x);
		usart_nadajbeznl(" y ");
		usart_nadaj_liczbe(y);
		usart_nadajbeznl(" z ");
		usart_nadaj_liczbe(z);
		usart_nadaj("");
	}
}
