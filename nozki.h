#ifndef NOZKI_H
#define NOZKI_H
#include <stm32.h>
#include <gpio.h>
#include <stddef.h>
#include <stdbool.h>

struct przerwanie {
	int GRUPA_PRZERWAN;
	int PIORYTET;
};

struct wyprowadzenie {
	int NOZKA;
	int ODWROCONY;
	char *komunikat;
	GPIO_TypeDef* PORT;
	bool naduszony;
	struct przerwanie pr;
};

extern const int LICZBA_WYPROWADZEN;


extern struct wyprowadzenie wy[];

extern bool jest_kukielka(int);

extern void ustaw_guziki(void);

extern bool guzik_wduszony(int guzik);

extern void ustaw_naduszony();

extern int byl_i_jest_wduszony();

extern int daj_wduszony_guzik();

extern bool jest_i2c(int);

#endif
