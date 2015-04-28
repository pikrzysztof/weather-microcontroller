#ifndef ZEGARY_H
#define ZEGARY_H
#include <stddef.h>

/* Inicjalizuje zegary, wlacza taktowanie itd. */
/* czestotliwosc jest dzielona przez 1000, dzieki czemu dla HSI_VALUE */
/* w ustaw_wywolanie dajemy milisekundy. */
extern void zegar_ustaw_poczatek(unsigned int czestotliwosc);

/* Ustawia funkcje i dane - to wywola sie przy alarmie. */
/* czas to ile razy musi tyknąć zegar żeby odpaliło się wywołanie. */
/* ile_razy < 0 oznacza, ze sie bedzie wywolywac w nieskonczonsoc */
extern void ustaw_wywolanie(int ile_razy, void (*wywolanie)(void*), void *dane, unsigned int czas);

#endif
