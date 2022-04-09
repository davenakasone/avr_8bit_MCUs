/*
	cpe301, da5, task3
	task1: speed of a DC motor is controlled by a potentiometer on PC0
	task2: speed of a stepper motor is controlled by a potentiometer on PC0
	task3: position of a servo is controlled by a potentiometer on PC0
	
	ADC0       --> PC0    // the potentiometer + gnd + AREF(5V)
	RX         --> PD0    // UART for viewing
	TX         --> PD1    // UART for viewing
	PWM_servo  --> PD2    // using OC3B

	preparations >>>
		[hammer] -> toolchain -> AVR/GNU Linker -> General -> check "Use vprintf library(-Wl, -u, vprintf)
		[hammer] -> toolchain -> AVR/GNU Linker -> Miscellaneous -> Other Linker Flags -> "-lprintf_flt"
		tools -> Data Visualizer -> Configuration -> External Connection -> Serial Port -> 
			set the terminal's BAUD to 9600, open a terminal, add /r/n, COM3 "mEDBG"
			
	external power used, shared ground
	
	servo, HS 5065MG, f = 200Hz, min: 750us, max: 2250us
	using timer3, prescaled x8, fast PWM mode 14
*/

#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE ((F_CPU / (BAUD * 16UL)) - 1)
#define HELP_BUF 128

#define ADC_MAX 1023
#define SERVO_WIDTH 10000    // f_pwm  = 200 Hz,  200 = f_cpu / (N * (TOP+1)  -> TOP = 9999
#define SERVO_START 1400     // 750us min,  f = f_cpu / N = 2e6, T = 1/f = 0.5us  ->  750/0.5 = 1500...don't max out
#define SERVO_STOP 4600      // 2250us max, "                                     "  2250/0.5 = 4500...don't max out
#define SERVO_HANG 100       // ms, change based on step size, the wait time
#define SERVO_RETURN 500     // time for reset
#define SERVO_RANGE (SERVO_STOP - SERVO_START)


#define PIN_SERVO 2    // servo uses PD2 for PWM
#define PIN_POT 0      // potentiometer on PC0

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>
#include <util/delay.h>

volatile unsigned char helper [HELP_BUF];
uint16_t adc_raw;
uint16_t servo_counter;

void read_adc (void);
void usart_putc (char send_char);
void usart_puts (const char* send_str);


int main(void)
{
	//int temp = 0;
	memset(helper, '\0', HELP_BUF);
	adc_raw = 0;
	servo_counter = SERVO_START;
	
	
	PORTC |= (1 << PIN_POT);                         // activate pull-up resistor for potentiometer reading
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    // 16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (0<<REFS0);                             // Voltage reference from AREF
	ADCSRA |= (1<<ADEN);                             // turn on ADC
	ADCSRA |= (1<<ADSC);                             // get first conversion done
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);       // turn on USART module, receive and transmit enabled
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);     // configure: asynchronous, 8-bit data, 1-bit stop
	UBRR0H = (BAUD_PRESCALE >> 8);               // sets baud rate, UBRR0 = { f_cpu / (16*BAUD) } - 1, has 4 msb
	UBRR0L = BAUD_PRESCALE;
	
	DDRD |= (1 << PIN_SERVO);                               // dedicated PWM pin for timer 3 as output
	PORTD |= (1 << PIN_SERVO);                              // this must be done to disable OC4B, the pin is shared
	ICR3 = SERVO_WIDTH;                                     // the "TOP", makes the time period, OCR3A makes duty cycle
	TCCR3A |= (1 << COM3B1) | (1 << WGM31);                 // non inverting, mode14
	TCCR3B |= (1 << WGM33) | (1 << WGM32) | (1 << CS31);    // mode 14, prescale 8
	
	usart_puts("\r\n");
	usart_puts("         initialization complete, program begins...\r\n");
	usart_puts("\r\n");
	sei();
	
	while(1)
	{
		read_adc();
		servo_counter = (SERVO_RANGE / ADC_MAX) * adc_raw + SERVO_START;
		if (servo_counter > SERVO_STOP ||
			servo_counter < SERVO_START )
		{
			servo_counter = SERVO_START;
			_delay_ms(SERVO_RETURN);
		}
		
		OCR3B = servo_counter;
		
		sprintf(helper, "adc=  %9d  ,  servo:  %9d\r\n", 
			adc_raw, servo_counter);
		usart_puts(helper);
		_delay_ms(SERVO_HANG);
	}
	return EXIT_FAILURE;
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
