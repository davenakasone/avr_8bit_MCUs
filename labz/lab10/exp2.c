/*
	lab10, exp2
	fast PWM, timer0, 5% and 55%
	
	timer0 uses PD6 for OC0A, put LED there
	timer0 is in mode 3, 0xFF top, OCR0A is manipulated
	
	f_pwm = f_cpu / (N * 256)
	Duty = OCR0A / 256 = 0.55, 0.5
*/
#define F_CPU 16000000UL
#define PWM_PIN 6  // OC0A on PD6
#define DUTY_55 

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
	// non-inverting, clear OC0A on compare-match, mode7, fast PWM, 0xFF top
	TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); 
	TCCR0B = (1 << CS02) | (1 << CS00);    // prescale 1024
	sei();
	
	#ifdef DUTY_55
		OCR0A = 140;
	#else
		OCR0A = 13;
	#endif
	
    while (1) {}
}


/////////~~~~~~~~END>  main.c
