/*
	lab10, exp3
	timer1, complementary toggle OC1A, OC1B
	
	a) timer1, T = 10 ms, f = 1/T = 100 Hz
		f_pwm = f_cpu / (N * [top+1])  --> ICR1 a top
		N = 8
		ICR1 = (f_cpu/(N*f_pwm)) - 1 --> ICR1 = 20000
		OCR1A, D = 5%, = 0.5ms = 1000
		OCR1B, D = 95%, = 9.5ms = 19000
	
	b) timer1, all the same, but T = 3 s
		f_pwm = 1/T = 0.33
		N = 1024
		ICR1 = (f_cpu/(N*f_pwm)) - 1 --> ICR1 = 46874
		OCR1A, D = 5%, = 150ms = 2343
		OCR1B, D = 95%, = 2.85s = 44530
	
	timer1 has:
		OC1A on PB1
		OC1B on PB2
*/
#define F_CPU 16000000UL
#define PWM_A 1    // OC1A is on PB1
#define PWM_B 2    // OC1B is on PB2
//#define T_10_MS 777 

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>
#include <util/delay.h>


int main(void)
{
	DDRB = (1 << PWM_A) | (1 << PWM_B);    // output, OC1A and OC1B
	TCNT1 = 0;                             // clear coutner
	
	// non-inverting, OC1A and OC1B clear on compare match
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << COM1B0) | (1 << WGM11);
	
	#ifdef T_10_MS
		ICR1 = 20000;
		OCR1A = 1000;
		OCR1B = 19000;
		// prescale 8, finish setting mode 14, fast PWM, 
		TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	#else
		ICR1 = 46874;
		OCR1A = 2343;
		OCR1B = 44530;
		// prescale 1024, finish setting mode 14, fast PWM,
		TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS10);
	#endif
	
	sei();
	
    while (1) {}
}


/////////~~~~~~~~END>  main.c

