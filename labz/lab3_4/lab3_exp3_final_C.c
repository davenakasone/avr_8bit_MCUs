/*
	lab3, exp3
	CTC mode using mode on timer0, PB0 as LED output
	input, DIPs [PC1, PC0]
	00: prescales 1024
	01: prescales 256
	10: prescales 64
	00: prescales 8
	
	checks TOV0 to indicate overflow, different inputs create different blink rates
*/
#define F_CPU 8000000UL    // for atmega328p

#define SET_00 0           // DIP state [00]
#define SET_01 1           // DIP state [01]
#define SET_10 0x2         // DIP state [10]
#define SET_11 0x3         // DIP state [11]

#define PS_1024 0x05       // prescale to 1024
#define PS_256 0x04        // prescale to 256
#define PS_64 0x03         // prescale to 64
#define PS_8 0x02          // prescale to 8

#define MY_LED 0           // using PB0 for output LED
#define DIP_0 0            // using PC0 as DIP switch 0
#define DIP_1 1            // using PC1 as DIP switch 1
#define LOOPZ 63           // need about 62 reps on 1024-prescaled timer0 to get 2 seconds
#define CALLZ 6            // calling a 2-second delay, this many times

#include <avr/io.h>
#include <stdlib.h>    // EXIT_FAILURE

void my_delay(uint8_t scaler);


int main
(
	void
)
{
    DDRB = (1 << MY_LED);           // set an output pin
	uint8_t led = 0;                // used to toggle LED
	uint8_t eor = (1 << MY_LED);    // used to toggle LED
	DDRC = 0;                       // PORTC as input
	PORTC = 0x3;                    // enable PC0, PC1 pull-up resistors
	PORTB = led;                    // start LED off
	uint8_t getter = 0;             // get values from DIPs
	
    while (1) 
    {
		getter = PINC;
		switch (getter)
		{
			case SET_00 : my_delay(PS_1024); break;
			case SET_01 : my_delay(PS_256); break;
			case SET_10 : my_delay(PS_64); break;
			case SET_11 : my_delay(PS_8); break;
			default: break; // hold value
		}
		led = led ^ eor;    // toggle value
		PORTB = led;        // LED toggles
    }
	
	return EXIT_FAILURE;
}


////~~~~


void my_delay(uint8_t scaler)
{
	uint8_t loopz = 0;  // tracks loops requested in LOOPZ
	TCNT0 = 0;          // clear timer0 counter
	TCCR0B = scaler;    // set prescale and timer0 begins
	while (loopz < LOOPZ)
	{
		while( (TIFR0 & 0x01) == 0) {} // wait for overflow
		loopz++;
		TCCR0B = 0;         // stop the timer
		TCNT0 = 0;          // clear timer0 counter
		TIFR0 = 0x01;       // reset TOV0
		TCCR0B = scaler;    // set prescale and timer0 begins
	}
	TCCR0B = 0;      // stop the timer
}


////////~~~~~~~~END>  main.c
