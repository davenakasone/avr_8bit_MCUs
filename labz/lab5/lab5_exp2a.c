/*
	lab 5, exp2a
	timer1 is in normal mode
	on overflow, the TOV1 interrupt toggles the LED every second
	f_out = f_cpu / (2 * pre * {TOP-start})
*/ 

#define F_CPU 8000000UL
//#define F_CPU 16000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>

#define MY_LED 5           // LED output on PB.5
#define CNT_START 3035     // TCNT1 begins here


int main
(
	void
)
{
    DDRB = (1 << MY_LED);                  // configure PB.5 as output
	PORTB = 0;                             // start with LEDs off
	TCNT1 = CNT_START;                     // configure TCNT1 with starting value
	TIMSK1 |= (1 << TOIE1);                // enable overflow interrupt on timer1
	TCCR1A = 0;                            // timer is configured for normal mode
	TCCR1B = (1 << CS11) | (1 << CS10);    //     using a prescaler of 64, count begins
	sei();                                 // enable interrupts

    while (1) {}    // the program hangs here

	return EXIT_FAILURE;
}


////~~~~


ISR(TIMER1_OVF_vect)           // executed on overflow
{
	PORTB ^= (1 << MY_LED);    // toggle the LED
	TCNT1 = CNT_START;         // the timer is reset, counting continues
}                              // TOV1 flag is reset in TIFR1


////////~~~~~~~~END>  main.c
