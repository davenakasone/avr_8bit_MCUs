/*
	lab 8, exp1
*/
#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>
#include <util/delay.h>

#define BUF_SIZ 256

char buffer[BUF_SIZ];    // output of the itoa function
             
void USART_init(void);           
void USART_send( unsigned char data);    
void USART_putstring(char* str_ptr);   


int main(void)
{
	memset(buffer, '\0', BUF_SIZ);        
	USART_init();        
	
	while(1)
	{        
		snprintf(buffer, BUF_SIZ, "USART Lab\r\n");
		USART_putstring(buffer);        // send coordinates
		USART_send('\n');     
		_delay_ms(1000); // limits data flow        
	}
	return EXIT_FAILURE;
}


////~~~~


void USART_init(void)
{	
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);    // set baud
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1 << TXEN0);                     // transmit enabled
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00);         // async, 8-bit, 1-bit stop	
}


////~~~~


void USART_send( unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0))){}// wait until ready
	UDR0 = data;
}


////~~~~


void USART_putstring(char* str)
{
	int ii = 0;
	while(str[ii] != '\0')
	{
		USART_send(str[ii]);
		ii++;
	}
}


////////~~~~~~~~END>  main.c