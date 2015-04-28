#include "i2c.h"
#include "usart.h"
#include <stm32.h>
#include <gpio.h>
#include <stdint.h>
#define I2C_SPEED_HZ 100000U 	/* 100 000 */

void ustaw_i2c(const unsigned int czestotliwosc)
{
	/* cala funkcja ze slajdow */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	GPIOafConfigure(GPIOB, 8, GPIO_OType_OD,
	                GPIO_Low_Speed, GPIO_PuPd_NOPULL, 4);
	GPIOafConfigure(GPIOB, 9, GPIO_OType_OD, GPIO_Low_Speed,
	                GPIO_PuPd_NOPULL, 4);
	I2C1->CR1 = 0;
	/* to bylo na wykladzie ile to ma byc */
	/* okres APB1 mierzone w ms */
	const unsigned int PCLK1_MHZ = czestotliwosc / (1000U * 1000U);
	I2C1->CR2 = PCLK1_MHZ;
	I2C1->CCR = (PCLK1_MHZ * 1000U * 1000U) / (I2C_SPEED_HZ << 1);
	I2C1->TRISE = PCLK1_MHZ + 1U;
	/* wlaczanie interfejsu */
	I2C1->CR1 |= I2C_CR1_PE;
}

/* wysyla start, adres, odczytuje poprawne */
void zacznij_transmisje(const uint8_t adres)
{
	I2C1->CR1 |= I2C_CR1_START;
	while ((I2C1->SR1 & I2C_SR1_SB) == 0) {
		;
	}
	I2C1->DR = adres << 1;
	while ((I2C1->SR1 & I2C_SR1_ADDR) == 0) {
		;
	}
	I2C1->SR2;
}

int8_t daj_zawartosc_rejestru(const uint8_t adres, const uint8_t rejestr)
{
	zacznij_transmisje(adres);
	I2C1->DR = rejestr;
	while ((I2C1->SR1 & I2C_SR1_BTF) == 0) {
		;
	}
	I2C1->CR1 |= I2C_CR1_START;
	while ((I2C1->SR1 & I2C_SR1_SB) == 0) {
		;
	}
	I2C1->DR = (adres << 1) | 1U;
	while ((I2C1->SR1 & I2C_SR1_ADDR) == 0) {
		;
	}
	I2C1->SR2;
	I2C1->CR1 &= ~I2C_CR1_ACK;
	while ((I2C1->SR1 & I2C_SR1_RXNE) == 0) {
		;
	}
	I2C1->CR1 |= I2C_CR1_STOP;
	return I2C1->DR;
}

void wpisz_do_rejestru(const uint8_t adres, const uint8_t rejestr, uint8_t wartosc)
{
	zacznij_transmisje(adres);
	I2C1->DR = rejestr;
	while ((I2C1->SR1 & I2C_SR1_TXE) == 0) {
		;
	}
	I2C1->DR = wartosc;
	while ((I2C1->SR1 & I2C_SR1_BTF) == 0) {
		;
	}
	I2C1->CR1 |= I2C_CR1_STOP;
}
