/*
	cpe301, da5, task2
	task1: speed of a DC motor is controlled by a potentiometer on PC0
	task2: speed of a stepper motor is controlled by a potentiometer on PC0
	task3: position of a servo is controlled by a potentiometer on PC0
	
	ADC0       --> PC0    // the potentiometer + gnd + AREF(5V)
	RX         --> PD0    // UART for viewing
	TX         --> PD1    // UART for viewing
	in1a       --> PB0    // stepper coil
	in1b       --> PB1    // stepper coil
	in2a       --> PB2    // stepper coil
	in2b       --> PB3    // stepper coil

	preparations >>>
		[hammer] -> toolchain -> AVR/GNU Linker -> General -> check "Use vprintf library(-Wl, -u, vprintf)
		[hammer] -> toolchain -> AVR/GNU Linker -> Miscellaneous -> Other Linker Flags -> "-lprintf_flt"
		tools -> Data Visualizer -> Configuration -> External Connection -> Serial Port -> 
			set the terminal's BAUD to 9600, open a terminal, add /r/n, COM3 "mEDBG"
			
	control the steps, 10ms to ?
	
	1a | 1b | 2a | 2b
	_________________
	1  | 0  | 0  | 0
	0  | 1  | 0  | 0
	0  | 0  | 1  | 0
	0  | 0  | 0  | 1
	...
*/
//#define TEST 3000
#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE ((F_CPU / (BAUD * 16UL)) - 1)
#define HELP_BUF 128
#define DEBUG_UART 123

#define ADC_MAX 1023          // highest raw ADC value, the minimum is 0
#define STEP_FASTEST 1500     // fastest by smallest pause between steps
#define STEP_SLOWEST 10000    // slowest by longest pause between steps
#define STEP_1 0x8
#define STEP_2 0x4
#define STEP_3 0x2
#define STEP_4 0x1

#define PINS_STEP 0xF   // the stepper coil pins on PB[0:3]
#define PIN_POT 0       // potentiometer on PC0

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>
#include <util/delay.h>

unsigned char helper[HELP_BUF];    // UART debugging
uint16_t adc_raw;                  // gets raw ADC value
uint16_t stepper_pause_us;         // amount of time to pause between step

void make_delay_us();
void read_adc (void);
void usart_putc (char send_char);
void usart_puts (const char* send_str);


int main(void)
{
	memset(helper, '\0', HELP_BUF);
	adc_raw = 0;
	stepper_pause_us = STEP_SLOWEST;
	
	PORTC |= (1 << PIN_POT);                         // activate pull-up resistor for potentiometer reading
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    // 16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (0<<REFS0);                             // Voltage reference from AREF
	ADCSRA |= (1<<ADEN);                             // turn on ADC
	ADCSRA |= (1<<ADSC);                             // get first conversion done
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);       // turn on USART module, receive and transmit enabled
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);     // configure: asynchronous, 8-bit data, 1-bit stop
	UBRR0H = (BAUD_PRESCALE >> 8);               // sets baud rate, UBRR0 = { f_cpu / (16*BAUD) } - 1, has 4 msb
	UBRR0L = BAUD_PRESCALE;
	
	DDRB = PINS_STEP;    // the stepper coil pins as output
	PORTB = 0;           // motor begins turned off
	
	usart_puts("\r\n");
	usart_puts("         initialization complete, program begins...\r\n");
	usart_puts("\r\n");
	sei();
	
	while(1)
	{
		read_adc();
		stepper_pause_us = (((STEP_FASTEST-STEP_SLOWEST)/ADC_MAX) * adc_raw) + STEP_SLOWEST;
		if (stepper_pause_us > STEP_SLOWEST || 
			stepper_pause_us < STEP_FASTEST  )
		{
			stepper_pause_us = STEP_SLOWEST;
		}
		PORTB = STEP_1;
		#ifdef TEST
			_delay_us(TEST);
		#else
			make_delay_us();
		#endif
		PORTB = STEP_2;
		#ifdef TEST
			_delay_us(TEST);
		#else
			make_delay_us();
		#endif
		PORTB = STEP_3;
		#ifdef TEST
			_delay_us(TEST);
		#else
			make_delay_us();
		#endif
		PORTB = STEP_4;
		#ifdef TEST
			_delay_us(TEST);
		#else
			make_delay_us();
		#endif
		
		#ifdef DEBUG_UART
			sprintf(helper, "adc_raw=  %9d  ,  stepper_pause_us=  %9d\r\n", 
				adc_raw, stepper_pause_us);
			usart_puts(helper);
		#endif
	}
	return EXIT_FAILURE;
}


////~~~~


void make_delay_us()
{
	uint16_t counter = 0;
	while (counter < stepper_pause_us)
	{
		_delay_us(1);
		counter++;
	}
}


////~~~~


void read_adc (void)
{
	ADMUX &= 0xF0;                   // clears channels
	ADMUX |= PIN_POT;                // set new channel to read
	ADCSRA |= (1 << ADSC);           // starts a new conversion
	while(ADCSRA & (1 << ADSC)){}    // wait until the conversion is done
	adc_raw = ADCW;                  // gets the ADC value
}


////~~~~


void usart_putc (char send_char)
{
	// wait until data register not empty
	while ((UCSR0A & (1 << UDRE0)) == 0) {}    
	UDR0 = send_char;
}


////~~~~


void usart_puts (const char* send_str)
{
	// send the string, char by char, until send_str[x] == NULL
	while (*send_str)
	{
		usart_putc(*send_str++);
	}
}


////////~~~~~~~~END>  main.c
