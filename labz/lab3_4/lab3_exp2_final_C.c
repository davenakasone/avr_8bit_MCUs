/*
	lab3, exp2
	timer0, normal, 2-second toggle
	checks TOV0 to indicate overflow
	multiple cycles of timer0 required
	using PB0 as LED output
*/
#define F_CPU 8000000UL    // for atmega328p
#define PRE_SCL 0x05       // prescale to 1024
#define MY_PIN 0           // using PB0
#define LOOPZ 63           // need about 62 reps on 1024-prescaled timer0 to get 2 seconds

#include <avr/io.h>
#include <stdlib.h>    // EXIT_FAILURE


int main
(
	void
)
{
    DDRB = (1 << MY_PIN);            // set an output pin
	uint16_t led = 0;                // used to toggle LED
	uint16_t eor = (1 << MY_PIN);    // used to toggle LED
	uint16_t step = 0;               // tracks steps needed for 2 seconds
	PORTB = led;                     // start LED off
	TCNT0 = 0;                       // clear timer0 counter
	TCCR0B = PRE_SCL;                // set prescale and timer0 begins
	
    while (1) 
    {
		while( (TIFR0 & 0x01) == 0) {} // wait for overflow
		
		TCCR0B = 0;      // stop the timer
		TCNT0 = 0;       // clear timer0 counter
		TIFR0 = 0x01;    // reset TOV0
		step++;
		
		if (step == LOOPZ)
		{
			step = 0;            // reset
			led = led ^ eor;     // toggle value
			PORTB = led;         // LED toggles
		}
		TCCR0B = PRE_SCL;    // set prescale and timer0 begins
    }
	
	return EXIT_FAILURE;
}


////////~~~~~~~~END>  main.c
