/* Define to prevent recursive inclusion */
#ifndef __UART_H
#define __UART_H

#include "stc15w2xx.h"
#include "intrins.h"

void UartInit(void);
void UART_String(uchar *s);
void UART_Char( uchar i );

#endif /* __UART_H */