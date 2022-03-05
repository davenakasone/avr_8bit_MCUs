/*
	lab 5, exp4
	fast PWM, 100 Hz
	OC1A and OC1B dim and brigten oppisite LEDs
	TCNT0 holds period
	OCR0A is duty cycle
*/ 

#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

const uint8_t LED_A = 1;          // OC1A is on PB.1
const uint8_t LED_B = 2;          // OC1B is on PB.2
volatile double dutyCycle = 0;    // controls duty cycle


int main
(
	void
)
{
    DDRB = (1 << LED_A);        // configure PORTB as output for LEDs
	DDRB |= (1 << LED_B);
	PORTB = 0;                  // start with LEDs off
	TIMSK1 = (1 << TOIE1);      // enable overflow interrupt
	OCR1A = 0;                  // start PWM off
	TCCR1A |= (1 << WGM10);     // preparing fast PWM, 8-bit
	TCCR1A |= (1 << COM1A1);    // clears OC1A on compare match, sets OC1A to bottom, non inverting
	TCCR1A |= (1 << COM1B1);    // clears OC1B on compare match, sets OC1B to bottom, non inverting
	TCCR1B |= (1 << WGM12);     // preparing fast PWM, 8-bit
	TCCR1B |= (1 << CS11);      // prescale to 64
	TCCR1B |= (1 << CS10);      // prescale to 64, timer is started
	sei();                      // enable interrupts

    while (1) 
	{
	}    

	return EXIT_FAILURE;
}


////~~~~


ISR(TIMER1_OVF_vect)    
{
	dutyCycle += 10;
	if (dutyCycle > 100)
	{
		dutyCycle = 0;    // reset if needed
	}
	OCR1A = (dutyCycle / 100) * 0xFFFF;    // duty cycle changes
}                         


////////~~~~~~~~END>  main.c
