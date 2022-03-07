/*
	DA3, task 3
    control LED with joystick
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

#define BUF_SIZ 512

char buffer[BUF_SIZ]; 

void adc_init(void);            
volatile uint16_t read_adc(uint8_t channel);  
void USART_init(void);           
void USART_send( unsigned char data);    
void USART_putstring(char* str_ptr);   
void timer1_update (volatile uint16_t freq_x, volatile uint16_t duty_y);


int main(void)
{
	DDRD = (1 << 5);                  // output LED is on PD.5
	PORTD = 0;                        // turn the LED off
	volatile uint16_t adc_val_x;      // ADC value, x position
	volatile uint16_t adc_val_y;      // ADC value, x position
	memset(buffer, '\0', BUF_SIZ);
	adc_init();        
	USART_init();      
	
	while(1)
	{      
		adc_val_x = read_adc(4);                // read x position
		adc_val_y = read_adc(5);                // read y position
		USART_putstring("\r\n");  
		timer1_update(adc_val_x, adc_val_y);    // sets PWM
	}
	return EXIT_FAILURE;
}


////~~~~


void timer1_update (volatile uint16_t freq_x, volatile uint16_t duty_y)
{
	uint16_t get_x = freq_x;
	uint16_t get_y = duty_y;
	// handle corner cases
	if (get_y == 0)
	{
		PORTD = 0;    // turn LED off, no duty cycle
		return;
	}
	if (get_y == 1023)
	{
		PORTD = (1 << 5);    // turn LED on, all on time
		return;
	}
	if (get_x < 0) {get_x = 1;}
	if (get_x > 1023) {get_x = 1022;}
	if (get_y < 1) {get_y = 1;}
	if (get_y > 1023) {get_y = 1022;}
	snprintf(buffer, BUF_SIZ, "[x,y]:  [%d, %d] ADC\r\n", get_x, get_y);
	USART_putstring(buffer);   
	
	// contain the frequency to range
	float f_frequency = 60 * (get_x / 1023.0);
	if (f_frequency > 60.0) {f_frequency = 60.0;}
	if (f_frequency < 1.0) {f_frequency = 1.0;}
	//
	//f_frequency = 30.0;
	snprintf(buffer, BUF_SIZ, "f_frequency:  %0.3f\r\n", f_frequency);
	USART_putstring(buffer);
	//
	
	// OCR1A = (F_CPU / (2 * N * f_osc)) - 1
	float f_compare_top = (16000000.0 / ( 256.0 * f_frequency)) - 1.0; 
	uint16_t compare_top = (uint16_t)f_compare_top;
	if (compare_top < 10) {compare_top = 10;}
	if (compare_top > 31250) {compare_top = 31250;}
	//
	//compare_top = 40000;
	snprintf(buffer, BUF_SIZ, "f_compare_top:  %0.3f\r\n", f_compare_top);
	USART_putstring(buffer);
	snprintf(buffer, BUF_SIZ, "compare_top:  %d\r\n", compare_top);
	USART_putstring(buffer);
	//

	float f_duty_cycle = ((float)get_y / 1023.0) * f_compare_top;
	uint16_t duty_cycle = (uint16_t)f_duty_cycle;
	if (duty_cycle < 5) {duty_cycle = 5;}
	if (duty_cycle > compare_top) {duty_cycle = compare_top;}
	//
	//duty_cycle = 20000;
	snprintf(buffer, BUF_SIZ, "f_duty_cycle:  %0.3f\r\n", f_duty_cycle);
	USART_putstring(buffer);
	snprintf(buffer, BUF_SIZ, "duty_cycle:  %d\r\n", duty_cycle);
	USART_putstring(buffer);
	//

	PORTD = (1 << 5);      // turn LED on
	OCR1A = compare_top;
	TIMSK1 = (1 << OCIE1A);                   // compareA flag
	TCNT1 = 0;
	TCCR1B |= 
		(1 << WGM12) |                        // ctc mode
		(1 << CS12)  |                        // prescale 256
		(0 << CS11)  | 
		(0 << CS10);                          // time starts
	while (TCNT1 < duty_cycle) {}             // wait on time
	PORTD = 0; // turn LED off
	while ((TIFR1 & (1 << OCF1A)) == 0) {}    // wait off time
	TIFR1 |= (1 << OCF1A);                    // clear flag
	TCCR1B = 0;                               // stop time
}


////~~~~


void adc_init(void)
{
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    // 16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (0<<REFS0);                             // Voltage reference from AREF
	ADCSRA |= (1<<ADEN);                             // turn on ADC
	ADCSRA |= (1<<ADSC);                             // get first conversion done
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
