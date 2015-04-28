#include "nozki.h"
#include <gpio.h>
#include <stddef.h>
#include <stdbool.h>

const int LICZBA_WYPROWADZEN = 14;

struct wyprowadzenie wy[] = {
	{.NOZKA = 0, .ODWROCONY = false, .komunikat = "at mode",
	 .PORT = GPIOA, .naduszony = false,
	 .pr = {.GRUPA_PRZERWAN = EXTI0_IRQn, .PIORYTET = 10}},

	{0},

	{0},

	{.NOZKA = 3, .ODWROCONY = true, .komunikat = "lewo",
	 .PORT = GPIOB, .naduszony = false,
	 .pr = {.GRUPA_PRZERWAN = EXTI3_IRQn, .PIORYTET = 10}},

	{.NOZKA = 4, .ODWROCONY = true, .komunikat = "prawo",
	 .PORT = GPIOB, .naduszony = false,
	 .pr = {.GRUPA_PRZERWAN = EXTI4_IRQn, .PIORYTET = 10}},

	{.NOZKA = 5, .ODWROCONY = true, .komunikat = "gora",
	 .PORT = GPIOB, .naduszony = false,
	 .pr = {.GRUPA_PRZERWAN = EXTI9_5_IRQn, .PIORYTET = 10}},

	{.NOZKA = 6, .ODWROCONY = true, .komunikat = "dol",
	 .PORT = GPIOB, .naduszony = false,
	 .pr = {.GRUPA_PRZERWAN = EXTI9_5_IRQn, .PIORYTET = 10}},

	{0},

	{0},

	{0},

	{.NOZKA = 10, .ODWROCONY = true, .komunikat = "fajer",
	 .PORT = GPIOB, .naduszony = false,
	 .pr = {.GRUPA_PRZERWAN = EXTI15_10_IRQn, .PIORYTET = 10}},

	{0},

	{0},

	{.NOZKA = 13, .ODWROCONY = true, .komunikat = "user",
	 .PORT = GPIOC, .naduszony = false,
	 .pr = {.GRUPA_PRZERWAN = EXTI15_10_IRQn, .PIORYTET = 10}},
};

bool jest_kukielka(int numer)
{
	return (numer < 0 || numer == 1 || numer == 2 || numer == 7
		|| numer == 8 || numer == 9 || numer == 11
		|| numer == 12);
}

void ustaw_guziki(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN
		| RCC_AHB1ENR_GPIOBEN
		| RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	for (int i = 0; i < LICZBA_WYPROWADZEN; ++i) {
		if (jest_kukielka(i))
			continue;
		struct wyprowadzenie tmp = wy[i];
		GPIOinConfigure(tmp.PORT, tmp.NOZKA,
				tmp.ODWROCONY ?
				GPIO_PuPd_UP : GPIO_PuPd_DOWN,
				EXTI_Mode_Interrupt,
				!tmp.ODWROCONY ?
				EXTI_Trigger_Rising :
				EXTI_Trigger_Falling);
	}
}

bool guzik_wduszony(int guzik)
{
	if (jest_kukielka(guzik)) {
		return false;
	}
	bool wduszony = (wy[guzik].PORT->IDR & (1U << wy[guzik].NOZKA)) == 0;
	if (wy[guzik].ODWROCONY == false)
		wduszony = !wduszony;
	return wduszony;
}

int daj_wduszony_guzik()
{
	for (int i = 0; i < LICZBA_WYPROWADZEN; ++i) {
		if (guzik_wduszony(i)) {
			return i;
		}
	}
	return -1;
}

void ustaw_naduszony()
{
	for (int i = 0; i < LICZBA_WYPROWADZEN; ++i) {
		if (!jest_kukielka(i)) {
			wy[i].naduszony = guzik_wduszony(i);
		}
	}
}

int byl_i_jest_wduszony()
{
	for (int i = 0; i < LICZBA_WYPROWADZEN; ++i) {
		if (!jest_kukielka(i) && wy[i].naduszony) {
			wy[i].naduszony = guzik_wduszony(i);
			if (wy[i].naduszony) {
				return i;
			}
		}
	}
	return -1;
}
