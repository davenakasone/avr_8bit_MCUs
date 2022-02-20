/*
	lab3, exp1
	timer1, normal, 8-second toggle
	uses polling of TCNT1x, translation of assembly
*/
#define F_CPU 8000000UL    // for atmega328p
#define CNT_HI 0x7A        // TCNT1H limit, high
#define CNT_LO 0x11        // TCNT1L limit, low
#define PRE_SCL 0x05       // prescale to 1024
#define MY_PIN 5           // using PB5

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
	PORTB = led;                     // start LED off
	TCNT1H = 0;                      // clear timer1 counter, high
	TCNT1L = 0;                      // clear timer1 counter, low
	TCCR1B = PRE_SCL;                // set prescale and timer1 begins
	
    while (1) 
    {
		if (TCNT1L == CNT_LO)
		{
			if (TCNT1H == CNT_HI)
			{
				TCCR1B = 0;          // stop timer1
				TCNT1H = 0;          // clear timer1 counter, high
				TCNT1L = 0;          // clear timer1 counter, low
				led = led ^ eor;     // toggle value
				PORTB = led;         // LED toggles
				TCCR1B = PRE_SCL;    // set prescale and timer1 begins
			}
		}
    }
	return EXIT_FAILURE;
}


////////~~~~~~~~END>  main.c
