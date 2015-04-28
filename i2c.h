#ifndef I2C_H_
#define I2C_H_
#include <inttypes.h>
extern void ustaw_i2c(const unsigned int czestotliwosc);

/* adres akcelerometru to LIS35DE_ADDR */
extern int8_t daj_zawartosc_rejestru(const uint8_t adres, const uint8_t rejestr);

extern void wpisz_do_rejestru(const uint8_t adres, const uint8_t rejestr, const uint8_t wartosc);

#endif
