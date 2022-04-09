/*
	lab10, exp1
	timer0, ctc, square wave
	
	timer0 uses PD6 for OC0A
	
	f_pwm = f_cpu / (2 * N * [1 + OCR0A])
	
	a)
		f_pwm = 4 MHz, T = 0.25 us
		OCR0A = 1
		N = 1
	
	b)
		f_pwm = 20 kHz, T = 50 us
		OCR0A = 49
		N = 8
*/
#define F_CPU 16000000UL
#define PWM_PIN 6  // OC0A on PD6
//#define PART_A

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>
#include <util/delay.h>


int main(void)
{
	DDRD = (1 << PWM_PIN);    // output, OC0A
	TCNT0 = 0;                // clear coutner
	
	// toggle OC0A, ctc mode 
	TCCR0A = (1 << COM0A0) | (1 << WGM01);
	TIMSK0 = (1 << OCIE0A); // for reset
	sei();
	
	#ifdef PART_A
		OCR0A = 1;
		TCCR0B = (1 << CS00); // no prescale
	#else
		OCR0A = 49;
		TCCR0B = (1 << CS01); // prescale to 8
	#endif
	
    while (1) {}
}


////~~~~


ISR (TIMER0_COMPA_vect)
{
	TCNT0 = 0;
}


/////////~~~~~~~~END>  main.c
