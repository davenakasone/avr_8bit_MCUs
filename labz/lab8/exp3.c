/*
	lab8, exp3
	
	interfacing with the ultra-sonic sensor
	
	PD7 for trigger
	PB0 for echo, must be use since timer1 has ICP1 on this pin
	PC4 for temperature sensor
	
	preparations >>>
	[hammer] -> toolchain -> AVR/GNU Linker -> General -> check "Use vprintf library(-Wl, -u, vprintf)
	[hammer] -> toolchain -> AVR/GNU Linker -> Miscellaneous -> Other Linker Flags -> "-lprintf_flt"
	tools -> Data Visualizer -> Configuration -> External Connection -> Serial Port ->
	set the terminal's BAUD to 9600, open a terminal, add \r\n, COM3 "mEDBG"
*/

#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE (((F_CPU / (BAUD * 16UL))) - 1)
#define HELP_BUF 128
#define DATA_BUF 8
#define TRUE 111
#define FALSE 100
#define ECHO_PIN 0       // on PORTB, must be on PB0 for ICR1
#define TRIGGER_PIN 7    // on PORTD
#define TEMP_PIN 4       // on PORTC, LM34

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <util/atomic.h>

volatile char helper[HELP_BUF];
volatile int timer_overflows;
volatile double distance;
volatile long tik_count;

volatile char helper[HELP_BUF];
volatile char data_in[DATA_BUF];
volatile char command_in;
volatile uint8_t data_count;
volatile uint8_t command_ready;
volatile uint16_t adc_val;
volatile int timer_overflows;
volatile double distance;
volatile double temperature;
volatile long tik_count;


void copy_command (void);
void read_adc (void);
void read_distance (void);
void usart_putc (char send_char);
void usart_puts (const char* send_str);
void help_screen (void);


int main ()
{
	
	// initialize program variables
	adc_val = 0;
	data_count = 0;
	tik_count = 0;
	distance = 0;
	temperature = 0;
	timer_overflows = 0;
	command_ready = FALSE;
	data_count = 0;
	memset(data_in, '\0', DATA_BUF);
	memset(helper, '\0', HELP_BUF);

	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    // 16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (0<<REFS0);                             // Voltage reference from AREF
	ADCSRA |= (1<<ADEN);                             // turn on ADC
	ADCSRA |= (1<<ADSC);                             // get first conversion done

	DDRD |= (1 << TRIGGER_PIN);    // for outputting trigger signal
	PORTB = 0xFF;                  // turn on pull up resistors for echo capture

	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);       // turn on USART module, receive and transmit enabled
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);     // configure: asynchronous, 8-bit data, 1-bit stop
	UBRR0H = (BAUD_PRESCALE >> 8);               // sets baud rate
	UBRR0L = BAUD_PRESCALE;
	UCSR0B |= (1 << RXCIE0 );                    // enable RX interrupt

	TIMSK1 = (1 << TOIE1);	// enable timer1 overflow interrupt
	TCCR1A = 0;		        // normal operation
	
	_delay_ms(100);
	usart_puts("\r\n");
	snprintf(helper, HELP_BUF-1, "temperature  :  %0.3lf\r\n", temperature);
	usart_puts(helper);
	snprintf(helper, HELP_BUF-1, "distance     :  %0.3lf\r\n", distance);
	usart_puts(helper);
	usart_puts("\r\n");
	usart_puts("         initialization complete, program begins...\r\n");
	usart_puts("\r\n");
	help_screen();
	sei();
	usart_puts("\r\n");
	usart_puts("initialized, program begins...\r\n");
	
	while (1)
	{
		if (command_ready == TRUE)
		{
			read_adc();
			read_distance();
			temperature = ((adc_val / 1023) * 500);
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
		
		if (command_in == 'a')
		{
			snprintf(helper, HELP_BUF-1, "%0.3lf degrees C\r\n",
			(temperature-32)*0.556);
			usart_puts(helper);
		}
		else if (command_in == 'b')
		{
			snprintf(helper, HELP_BUF-1, "%lf degrees F\r\n",
			temperature);
			usart_puts(helper);
		}
		else if (command_in == 'c')
		{
			snprintf(helper, HELP_BUF-1, "%lf  cm\n",
			distance);
			usart_puts(helper);
		}
		else if (command_in == 'd')
		{
			snprintf(helper, HELP_BUF-1, "%lf  in\n",
			distance/2.54);
			usart_puts(helper);
		}
		else
		{
			usart_puts("invalid entry, try again\r\n");
		}
		help_screen();
	}
}


////~~~~


void read_distance (void)
{
	timer_overflows = 0;                             // reset overflow counter
	// give 10us pulse to the trigger pin
	PORTD |= (1 << TRIGGER_PIN);                     // pulse begin
	_delay_us(10);                                   // wait 10us
	PORTD &= ~(1 << TRIGGER_PIN);                    // pulse end
	TCNT1 = 0;                                       // reset timer1 counter
	if (TIFR1 & (1 << ICF1))
	{
		TIFR1 = 1<<ICF1;                             // clear input capture flag
	}
	if (TIFR1 & (1 << TOV1))
	{
		TIFR1 = 1<<TOV1;                             // clear overflow flag
	}
	TCCR1B = 0x41;                                   // capture on rising edge, no prescaler
	// calculate echo width by input capture
	while ((TIFR1 & (1 << ICF1)) == 0) {}            // wait for rising edge
	if (TIFR1 & (1 << ICF1))
	{
		TIFR1 = (1 << ICF1);                         // clear input capture flag
	}
	if (TIFR1 & (1 << TOV1))
	{
		TIFR1 = (1 << TOV1);                         // clear overflow flag
	}
	TCCR1B = 0x01;                                   // capture on falling edge, no prescaler
	while ((TIFR1 & (1 << ICF1)) == 0) {}            // wait for falling edge
	// the distance is ready
	tik_count = ICR1 + (65535 * timer_overflows);    // get total tiks
	distance = (double)tik_count / 933;              // 16 MHz timer, 343 m/s speed of sound
}


////~~~~


void read_adc (void)
{
	ADMUX &= 0xF0;                 // clears channels
	ADMUX |= TEMP_PIN;             // set new channel to read
	ADCSRA |= (1<<ADSC);           // starts a new conversion
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


void help_screen (void)
{
	usart_puts("\r\n");
	usart_puts("\tS_TEMP_C, get temperature, C   :  [ a ]\r\n");
	usart_puts("\tS_TEMP_F, get temperature, F   :  [ b ]\r\n");
	usart_puts("\tS_TEMP_CM, get distance, CM    :  [ c ]\r\n");
	usart_puts("\tS_TEMP_IN, get distance, IN    :  [ d ]\r\n");
	usart_puts("enter a char: ");
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


////~~~~


ISR(TIMER1_OVF_vect)
{
	timer_overflows++;
}


////////~~~~~~~~END>  main.c
