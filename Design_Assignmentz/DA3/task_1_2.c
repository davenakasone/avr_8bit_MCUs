/*
	furthering DA3, task 1:2
	joy stick with UART display
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

void adc_init(void);            
volatile uint16_t read_adc(uint8_t channel);  
void USART_init(void);           
void USART_send( unsigned char data);    
void USART_putstring(char* str_ptr);   


int main(void)
{
	volatile uint16_t adc_val_x;      // ADC value, x position
	volatile uint16_t adc_val_y;      // ADC value, x position
	volatile float val_x;             // voltage on x position
	volatile float val_y;             // voltage on y position
	memset(buffer, '\0', BUF_SIZ);
	adc_init();        
	USART_init();        
	
	while(1)
	{        
		adc_val_x = read_adc(4);        // read x position
		val_x = 5.0 * ((float)adc_val_x / 1023.0);    // find the voltage, x position
		adc_val_y = read_adc(5);        // read y position
		val_y = 5.0 * ((float)adc_val_y / 1023.0);    // find the voltage, y position
		snprintf(buffer, BUF_SIZ, "[x,y]:  [%d, %d] ADC  ,  [%0.3f, %0.3f] volts\r\n",
			adc_val_x, adc_val_y, val_x, val_y);
		USART_putstring(buffer);        // send coordinates
		USART_send('\n');     
		_delay_ms(1000);                // limits data flow        
	}
	return EXIT_FAILURE;
}


////~~~~


void adc_init(void)
{
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    //16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (0<<REFS0);                //Voltage reference from AREF
	ADCSRA |= (1<<ADEN);                // turn on ADC
	ADCSRA |= (1<<ADSC);                //initial conversion because this one is the slow
}


////~~~~


volatile uint16_t read_adc(uint8_t channel)
{
	ADMUX &= 0xF0;                 // clears channels
	ADMUX |= channel;              // set new channel to read
	ADCSRA |= (1<<ADSC);           // starts a new conversion
	while(ADCSRA & (1<<ADSC)){}    // wait until the conversion is done
	return ADCW;                   // returns the ADC value of the chosen channel
}


////~~~~


void USART_init(void)
{
	//UBRR0L = F_CPU/16/BAUD_RATE-1;           // the baud rate is set	
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
