/*
	lab 8, exp 2
*/

#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)
#define DATA_BUF 8
#define HELP_BUF 128
#define TRUE 111
#define FALSE 100

#include <avr/interrupt.h>
#include <avr/io.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>

volatile unsigned char helper [HELP_BUF];
volatile unsigned char data_in[DATA_BUF];
volatile unsigned char command_in;
volatile uint8_t data_count;
volatile uint8_t command_ready;

void usart_putc (char send_char);
void usart_puts (const char* send_str);
void copy_command (void);
void help_screen (void);


int main(void)
{
	// initialize the program variables
	command_ready = FALSE;
	memset(data_in, '\0', DATA_BUF);
	data_count = 0;
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);       // turn on USART module, receive and transmit enabled
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);     // configure: asynchronous, 8-bit data, 1-bit stop
	UBRR0H = (BAUD_PRESCALE >> 8);               // sets baud rate
	UBRR0L = BAUD_PRESCALE;
	UCSR0B |= (1 << RXCIE0 );                    // enable RX interrupt
	usart_puts("\r\n");
	usart_puts("         initialization complete, program begins...\r\n");
	usart_puts("\r\n");
	sei();                                    
	help_screen();
	
	while(1)
	{
		if (command_ready == TRUE)
		{
			copy_command();
			command_ready = FALSE;
		}
	}
	return EXIT_FAILURE;
}


////~~~~


void copy_command (void)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		command_in = data_in[0];
		memset(data_in, '\0', DATA_BUF);
		usart_puts("\r\n");
		snprintf(helper, HELP_BUF-1, 
			"    you entered:  %c\r\n", command_in);
		usart_puts(helper);
		usart_puts("\r\n");
		if (command_in >= 65 && command_in <= 90) 
		{
			command_in = tolower(command_in);
			snprintf(helper, HELP_BUF-1, "%c\r\n", command_in);
			usart_puts(helper);
		}
		else if (command_in >= 97 && command_in <= 122)
		{
			command_in = toupper(command_in);
			snprintf(helper, HELP_BUF-1, "%c\r\n", command_in);
			usart_puts(helper);
		}
		else
		{
			usart_puts("invalid entry\r\n");
		}
		help_screen();
		
	}
}


////~~~~


void help_screen (void)
{
	usart_puts("\r\n");
	usart_puts("enter a char: ");
}


////~~~~


void usart_putc (char send_char)
{
	while ((UCSR0A & (1 << UDRE0)) == 0) {}
	UDR0 = send_char;
}


////~~~~


void usart_puts (const char* send_str)
{
	while (*send_str)
	{
		usart_putc(*send_str++);
	}
}


////~~~~



ISR (USART0_RX_vect)
{
	data_in[data_count] = UDR0;
	if (data_in[data_count] == '\n')
	{
		command_ready = TRUE;
		data_count = 0;
	}
	else
	{
		data_count++;
	}
}


////////~~~~~~~~END>  main.c
