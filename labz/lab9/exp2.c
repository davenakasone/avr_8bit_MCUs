/*
	lab9, exp2
	the ADC raw value is read by interrupt
	UART is used for verification
	a volt meter confirmed the reading
	this was adapted to the LCD in exp3

	PC0 -> middle leg of potentiometer
	AREF -> postive leg of potentiometer
	gnd -> negative leg of potentiometer
	connect VCC to AREF with a small resistor
		no output pins required
*/

#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)
#define HELP_BUF 128
#define POT_PIN 0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <util/atomic.h>

volatile char helper[HELP_BUF];
volatile uint16_t adc_val;

void read_adc (void);
void usart_putc (char send_char);
void usart_puts (const char* send_str);


int main(void)
{
	// initialize program variables
	adc_val = 0;
	memset(helper, '\0', HELP_BUF);

	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    // 16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (0<<REFS0);                             // Voltage reference from AREF
	ADCSRA |= (1<<ADEN);                             // turn on ADC
	ADCSRA |= (1<<ADSC) |(1<<ADIE);                  // get first conversion done
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);       // turn on USART module, receive and transmit enabled
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);     // configure: asynchronous, 8-bit data, 1-bit stop
	UBRR0H = (BAUD_PRESCALE >> 8);               // sets baud rate
	UBRR0L = BAUD_PRESCALE;

	PORTC |= (1 << POT_PIN);    // turn on ADC pullup
	
	sei();

    while (1) {}    // wait here, ISR handles everything
}


////~~~~


void read_adc (void)
{
	ADMUX &= 0xF0;                 // clears channels
	ADMUX |= POT_PIN;              // set new channel to read
	ADCSRA |= (1 << ADSC);         // starts a new conversion
	while(ADCSRA & (1<<ADSC)){}    // wait until the conversion is done
	adc_val = ADCW;                // gets the ADC value
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


ISR (ADC_vect)
{
	read_adc();
	snprintf(helper, HELP_BUF-1, "raw adc:  %d", adc_val);
	usart_puts("\r\n");
	usart_puts(helper);
	_delay_ms(2000);    // 2 second pause to limit output
}


////////~~~~~~~~END>  main.c
