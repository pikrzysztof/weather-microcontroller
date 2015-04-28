#include "zegary.h"
#include <stdbool.h>
#include <stddef.h>
#include <stm32.h>
#include "usart.h"
#define ROZMIAR_TABLICY_PRZERWAN 10

typedef void (*cb) (void*);
static bool sensowne[ROZMIAR_TABLICY_PRZERWAN];
static cb fn[ROZMIAR_TABLICY_PRZERWAN];
static void* dane[ROZMIAR_TABLICY_PRZERWAN];
static int ile_razy[ROZMIAR_TABLICY_PRZERWAN];
static int czas[ROZMIAR_TABLICY_PRZERWAN];
static int pozostalo[ROZMIAR_TABLICY_PRZERWAN];

/* rejestry:  */
/* counter register: TIMx_CNT  */
/* prescaler register, TIMx_PSC*/
/* Auto-Reload Register TIMx_ARR */
void zegar_ustaw_poczatek(unsigned int czestotliwosc)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = 0;

	TIM3->ARR = czestotliwosc / 1000;

	/* DMA interrupt enable register */
	TIM3->DIER |= TIM_DIER_UIE;

	NVIC_EnableIRQ(TIM3_IRQn);

	/* Counter enabled bit */
	TIM3->CR1 |= TIM_CR1_CEN;
}



void ustaw_wywolanie(int ile_razy_zawolac, cb wywolanie, void *dane_cb, unsigned int czas_cb)
{
	for (int i = 0; i < ROZMIAR_TABLICY_PRZERWAN; ++i) {
		if (sensowne[i] != true) {
			sensowne[i] = true;
			fn[i] = wywolanie;
			dane[i] = dane_cb;
			ile_razy[i] = ile_razy_zawolac;
			pozostalo[i] = czas_cb;
			czas[i] = czas_cb;
			break;
		}
	}
}

void TIM3_IRQHandler()
{
	TIM3->SR = ~TIM_SR_UIF;
	for (int i = 0; i < ROZMIAR_TABLICY_PRZERWAN; ++i) {
		if (sensowne[i] == true) {
			pozostalo[i]--;
			if (pozostalo[i] == 0) {
				/* skonczylismy odliczac, uruchamiamy callback */
				fn[i](dane[i]);
				if (ile_razy[i] > 0) {
					ile_razy[i]--;
				}
				if (ile_razy[i] != 0) {
					pozostalo[i] = czas[i];
				} else {
					/* wykolejkowanie */
					sensowne[i] = false;
				}
			}
		}
	}
}
