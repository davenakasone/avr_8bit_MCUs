/*
	cpe310L, lab 11, exp1a
	the DC motor moves CCW, CW, or stops
	
	ADC0       --> PC0    // the potentiometer + gnd + AREF(5V)
	RX         --> PD0    // UART for viewing
	TX         --> PD1    // UART for viewing
	PWM_dc     --> PB2    // using OC1B
	in1_dc     --> PB1    // motor direction control
	in2_dc     --> PB0    // motor direction control
	CW         --> PD7    // move the motor clockwise
	CCW        --> PD6    // move the motor counter clockwise
	HALT       --> PD5    // the motor stops 
			
	external power used (6V), shared ground
	DC motor has max of 6V
	no direction change required
	
	 in1  |  in2  |  action
	______________________
	  0   |   0   |  stop
	  0   |   1   |  CW
	  1   |   0   |  CCW
	  1   |   1   | illegal

	given f_pwm = 100, T = 10 ms, N = 64
	f_pwm = f_cpu / (N * [1 + ICR1]),, --> ICR1 as top
*/

#define F_CPU 16000000UL

#define DC_WIDTH 2499    // sets ICR1 to get required frequency
#define DC_D 800        // the operating duty cycle of the motor

#define DC_PWM 2     // motor enable using PWM, PB2
#define DC_IN_1 1    // controls motor direction, PB1
#define DC_IN_2 0    // controls motor direction, PB0
#define PIN_CW 7     // control motor, CW, PD7
#define PIN_CCW 6    // control motor, CCW, PD6
#define PIN_HALT 5   // control motor, stop, PD5

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>
#include <util/delay.h>


int main(void)
{
	DDRD &= ~(1 << PIN_CCW);
	DDRD &= ~(1 << PIN_CW);
	DDRD &= ~(1 << PIN_HALT);
	PORTD |= (1 << PIN_CCW) | (1 << PIN_CW) | (1 << PIN_HALT); // turn on pullups for controller
	
	DDRB = (1 << DC_PWM) | (1 << DC_IN_1) | (1 << DC_IN_2);              // outputs to motor driver
	PORTB |= (1 << DC_IN_1);                                             // direction is set
	ICR1 = DC_WIDTH;                                                     // frequency of motor PWM enable is set
	OCR1B = DC_D;                                                        // duty cycle is set
	TCCR1A = (1 << COM1B1) | (1 << WGM11);                               // timer1, fast PWM, mode 14, non-inverting, OC1B duty cycle
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);    // prescale 64
	
	while(1)
	{	
		if (PIND & PIN_CW)
		{
			OCR1B = DC_D;
			PORTB |= (1 << DC_IN_1);
			PORTB &= ~(1 << DC_IN_2);
			_delay_ms(1000);
		}
		else if (PIND & PIN_CCW)
		{
			OCR1B = DC_D;
			PORTB &= ~(1 << DC_IN_1);
			PORTB |= (1 << DC_IN_2);
			_delay_ms(1000);
		}
		else
		{
			OCR1B = 0;
			PORTB &= ~(1 << DC_IN_1);
			PORTB &= ~(1 << DC_IN_2);
			_delay_ms(1000);
		}
		_delay_ms(1000);
	}
	return EXIT_FAILURE;
}

////////~~~~~~~~END>  main.c

