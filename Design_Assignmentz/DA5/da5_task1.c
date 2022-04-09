/*
	cpe301, da5, task1
	task1: speed of a DC motor is controlled by a potentiometer on PC0
	task2: speed of a stepper motor is controlled by a potentiometer on PC0
	task3: position of a servo is controlled by a potentiometer on PC0
	
	ADC0       --> PC0    // the potentiometer + gnd + AREF(5V)
	RX         --> PD0    // UART for viewing
	TX         --> PD1    // UART for viewing
	PWM_dc     --> PB2    // using OC1B
	in1_dc     --> PB1    // motor direction control
	in2_dc     --> PB0    // motor direction control

	preparations >>>
		[hammer] -> toolchain -> AVR/GNU Linker -> General -> check "Use vprintf library(-Wl, -u, vprintf)
		[hammer] -> toolchain -> AVR/GNU Linker -> Miscellaneous -> Other Linker Flags -> "-lprintf_flt"
		tools -> Data Visualizer -> Configuration -> External Connection -> Serial Port -> 
			set the terminal's BAUD to 9600, open a terminal, add /r/n, COM3 "mEDBG"
			
	external power used (6V), shared ground
	DC motor has max of 6V
	keep duty cycle 5% : 95% , T = 20ms : 30ms
	no direction change required
	
	 in1  |  in2  |  action
	______________________
	  0   |   0   |  stop
	  0   |   1   |  CW
	  1   |   0   |  CCW
	  1   |   1   | illegal

	T = 25ms, f_pwm = 1/T = 40 Hz, N = 64
	f_pwm = f_cpu / (N * [1 + ICR1]),, --> ICR1 as top @ 6249
*/

#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE ((F_CPU / (BAUD * 16UL)) - 1)
#define HELP_BUF 128
#define DEBUG_UART 123

#define ADC_MAX 1023     // highest raw ADC value, the minimum is 0
#define DC_WIDTH 6249    // sets ICR1 to get required frequency
#define DC_MIN 0         // for motor enable, 0% duty cycle
#define DC_MAX 6138      // for motor enable, 98% duty cycle ... maps 6x

#define DC_PWM 2     // motor enable using PWM, PB2
#define DC_IN_1 1    // controls motor direction, PB1
#define DC_IN_2 0    // controls motor direction, PB0
#define PIN_POT 0    // potentiometer on PC0

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>
#include <util/delay.h>

unsigned char helper[HELP_BUF];    // UART debugging
uint16_t adc_raw;                  // gets raw ADC value
uint16_t motor_duty;               // sets duty cycle using OCR1B of motor PWM enable

void read_adc (void);
void usart_putc (char send_char);
void usart_puts (const char* send_str);


int main(void)
{
	memset(helper, '\0', HELP_BUF);
	adc_raw = 0;
	motor_duty = DC_MIN;
	
	PORTC |= (1 << PIN_POT);                         // activate pull-up resistor for potentiometer reading
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    // 16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (0<<REFS0);                             // Voltage reference from AREF
	ADCSRA |= (1<<ADEN);                             // turn on ADC
	ADCSRA |= (1<<ADSC);                             // get first conversion done
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);       // turn on USART module, receive and transmit enabled
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);     // configure: asynchronous, 8-bit data, 1-bit stop
	UBRR0H = (BAUD_PRESCALE >> 8);               // sets baud rate, UBRR0 = { f_cpu / (16*BAUD) } - 1, has 4 msb
	UBRR0L = BAUD_PRESCALE;
	
	DDRB = (1 << DC_PWM) | (1 << DC_IN_1) | (1 << DC_IN_2);              // outputs to motor driver
	PORTB |= (1 << DC_IN_1);                                             // direction is set
	ICR1 = DC_WIDTH;                                                     // frequency of motor PWM enable is set
	TCCR1A = (1 << COM1B1) | (1 << WGM11);                               // timer1, fast PWM, mode 14, non-inverting, OC1B duty cycle
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);    // prescale 64
	
	usart_puts("\r\n");
	usart_puts("         initialization complete, program begins...\r\n");
	usart_puts("\r\n");
	sei();
	
	while(1)
	{
		read_adc();
		motor_duty = 6 * adc_raw;
		if (motor_duty > DC_MAX || motor_duty < DC_MIN)
		{
			motor_duty = DC_MIN;
		}
		OCR1B = motor_duty;
		
		#ifdef DEBUG_UART
			sprintf(helper, "adc_raw=  %9d  ,  motor_duty=  %9d\r\n", 
				adc_raw, motor_duty);
			usart_puts(helper);
		#endif
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