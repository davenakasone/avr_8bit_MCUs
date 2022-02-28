/*
	lab3, exp4
	timer0, normal, 12-second toggle, just call the 2-second delay 6 times
	checks TOV0 to indicate overflow
	multiple cycles of timer0 required, using 2-second loop with 6 calls
	using PB0 as LED output
*/
#define F_CPU 8000000UL    // for atmega328p
#define PRE_SCL 0x05       // prescale to 1024
#define MY_PIN 0           // using PB0
#define LOOPZ 63           // need about 62 reps on 1024-prescaled timer0 to get 2 seconds
#define CALLZ 6            // calling a 2-second delay, this many times

#include <avr/io.h>
#include <stdlib.h>    // EXIT_FAILURE

void delay_2_sec(void);


int main
(
	void
)
{
    DDRB = (1 << MY_PIN);           // set an output pin
	uint8_t led = 0;                // used to toggle LED
	uint8_t eor = (1 << MY_PIN);    // used to toggle LED
	uint8_t callz = 0;              // controls calls made to 2-second delay
	PORTB = led;                    // start LED off
	
    while (1) 
    {
		while (callz < CALLZ)    // complete 6 calls
		{
			callz++;
			delay_2_sec();    // wait 2 seconds
		}
		callz = 0;          // reset
		led = led ^ eor;    // toggle value
		PORTB = led;        // LED toggles
    }
	
	return EXIT_FAILURE;
}


////~~~~


void delay_2_sec(void)
{
	uint8_t loopz = 0;       // tracks loops needed for 2 seconds
	TCCR0B = PRE_SCL;        // set prescale and timer0 begins
	TCNT0 = 0;               // clear timer0 counter
	while (loopz < LOOPZ)
	{
		while( (TIFR0 & 0x01) == 0) {} // wait for overflow
		loopz++;
		TCCR0B = 0;          // stop the timer
		TCNT0 = 0;           // clear timer0 counter
		TIFR0 = 0x01;        // reset TOV0
		TCCR0B = PRE_SCL;    // set prescale and timer0 begins
	}
	TCCR0B = 0;    // stop the timer
}


////////~~~~~~~~END>  main.c
