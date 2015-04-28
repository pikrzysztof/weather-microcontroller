#ifndef USART_H_KRZYSZTOF_
#define USART_H_KRZYSZTOF_
#include <stdint.h>

extern void usart_ustaw(void);

extern void usart_nadajbeznl(char *tekst);

extern void usart_nadaj(char *tekst);

extern void usart_nadaj_liczbe(uint8_t ile);



#endif
