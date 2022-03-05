/*
	lab 5, exp1
	blinks LED fast, on INT0 {PD2}, rising edge
	DIP switch activates the interrupt
    LED is on {PB5}, it blinks slow if the interrupt is not detected
	if the interrupt is detected, the program remains in the ISR
*/ 

#define F_CPU 8000000UL
//#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define MY_LED 5           // LED output on PB.5
#define MY_DIP 2           // DIP switch input on PD.2, where INT0 is
#define DELAY_FAST 200     // fast blink, 5 times per second
#define DELAY_SLOW 2000    // slow blink, 1 time every 2 seconds


int main
(
	void
)
{
    DDRB = (1 << MY_LED);                   // configure PB.5 as output
	PORTB = 0;                              // start with LEDs off
	DDRD = (0 << MY_DIP);                   // configure PD.2 as input
	PORTD |= (1 << MY_DIP);                 // turn on pull-up resistors on PORTD
	EIMSK = (1 << INT0);                    // enable INT0
	EICRA = (1 << ISC01) | (1 << ISC00);    // configure INT0 for rising edge trigger
	sei();                                  // enable interrupts

    while (1) 
    {
		_delay_ms(DELAY_SLOW);	   // wait for a long time
		PORTB ^= (1 << MY_LED);    // toggle the LED
    }

	return EXIT_FAILURE;
}


////~~~~


ISR(INT0_vect)
{
	while (PIND & (1<< MY_DIP))    // check switch state, stay here while on
	{
		_delay_ms(DELAY_FAST);	   // wait for a short time
		PORTB ^= (1 << MY_LED);    // toggle the LED
	}
}


////////~~~~~~~~END>  main.c