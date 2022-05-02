#include "ICM20948.h"

void USART_init(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}


////~~~~


void USART_tx_string(char* data)
{
	while((*data != '\0'))
	{
		while(!(UCSR0A & (1<<UDRE0))){}
		UDR0 = *data;
		data++;
	}
}


////////~~~~~~~~END>  uart_support.c
