#include <stm32.h>
#include <gpio.h>
#include <stdint.h>
#include "diodki.h"

#define TIM3_CCMR


void ustaw_pwm()
{
	ustaw_diodki();
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	GPIOafConfigure(GPIOA, 6, GPIO_OType_PP,
	                GPIO_Low_Speed, GPIO_PuPd_NOPULL, 2);
	GPIOafConfigure(GPIOA, 7, GPIO_OType_PP,
	                GPIO_Low_Speed, GPIO_PuPd_NOPULL, 2);
	GPIOafConfigure(GPIOB, 0, GPIO_OType_PP,
			GPIO_Low_Speed, GPIO_PuPd_NOPULL, 2);

	TIM3->PSC = 16;
	TIM3->ARR = 130;

	TIM3->CCMR1 =
		TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE |
		TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;

	TIM3->CCMR2 =
		TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;

	TIM3->CCER =
		TIM_CCER_CC1E | TIM_CCER_CC1P |
		TIM_CCER_CC2E | TIM_CCER_CC2P |
		TIM_CCER_CC3E | TIM_CCER_CC3P;
	/* for (int i = 0; i < 3; ++i) { */
	/* 	zmien_stan_diodki(i, -1); */
	/* } */
	TIM3->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;
}

void wypelnij(uint8_t czerwona, uint8_t zielona, uint8_t niebieska)
{
	TIM3->CCR1 = czerwona;
	TIM3->CCR2 = zielona;
	TIM3->CCR3 = niebieska;
}

void ustaw_diodki(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN |
		RCC_AHB1ENR_GPIOBEN;
	__NOP();
	GPIOoutConfigure(GPIOA, 6, GPIO_OType_PP,
			 GPIO_Low_Speed, GPIO_PuPd_NOPULL);
	GPIOoutConfigure(GPIOA, 7, GPIO_OType_PP,
			 GPIO_Low_Speed, GPIO_PuPd_NOPULL);
	GPIOoutConfigure(GPIOB, 0, GPIO_OType_PP,
			 GPIO_Low_Speed, GPIO_PuPd_NOPULL);
}

/* numery od 0, stan > 0 to wlaczenie */
void zmien_stan_diodki(int numer_diodki, int stan)
{
	if (numer_diodki == 0) {
		if (stan > 0) {
			GPIOA->BSRRH = 1 << 6;
		} else {
			GPIOA->BSRRL = 1 << 6;
		}
	} else if (numer_diodki == 1) {
		if (stan > 0) {
			GPIOA->BSRRH = 1 << 7;
		} else {
			GPIOA->BSRRL = 1 << 7;
		}
	} else if (numer_diodki == 2) {
		if (stan > 0) {
			GPIOB->BSRRH = 1 << 0;

		} else {
			GPIOB->BSRRL = 1 << 0;
		}
	}
}
