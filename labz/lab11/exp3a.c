/*
	cpe310L, lab 11, exp 3
	servo angle controlled by DIP
	
	ADC0       --> PC0        // the potentiometer + gnd + AREF(5V)
	PWM_servo  --> PD2        // using OC3B
	DIP[2:0]   --> PB[2:0]    // DIP switches

	preparations >>>
		[hammer] -> toolchain -> AVR/GNU Linker -> General -> check "Use vprintf library(-Wl, -u, vprintf)
		[hammer] -> toolchain -> AVR/GNU Linker -> Miscellaneous -> Other Linker Flags -> "-lprintf_flt"
		tools -> Data Visualizer -> Configuration -> External Connection -> Serial Port -> 
			set the terminal's BAUD to 9600, open a terminal, add /r/n, COM3 "mEDBG"
			
	external power used, shared ground
	
	servo, HS 5065MG, f = 200Hz, min: 750us, max: 2250us
	using timer3, prescaled x8, fast PWM mode 14
*/
#define F_CPU 16000000UL

#define ADC_MAX 1023
#define SERVO_WIDTH 10000    // f_pwm  = 200 Hz,  200 = f_cpu / (N * (TOP+1)  -> TOP = 9999
#define SERVO_START 1400     // 750us min,  f = f_cpu / N = 2e6, T = 1/f = 0.5us  ->  750/0.5 = 1500...don't max out
#define SERVO_STOP 4600      // 2250us max, "                                     "  2250/0.5 = 4500...don't max out
#define SERVO_HANG 100       // ms, change based on step size, the wait time
#define SERVO_RETURN 500     // time for reset
#define SERVO_RANGE (SERVO_STOP - SERVO_START)

// servo PWM corresponding to inputs
#define SERVO_001 1530
#define SERVO_010 3700
#define SERVO_011 2550
#define SERVO_100 4600
#define SERVO_101 1400
#define SERVO_110 4466
#define SERVO_111 3700

#define PIN_SERVO 2    // servo uses PD2 for PWM
#define sBIT_0 0        // input bit 0, PB0
#define sBIT_1 1        // input bit 1, PB1
#define sBIT_2 2        // input bit 2, PB2

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>
#include <util/delay.h>

uint16_t servo_counter;


int main(void)
{
	servo_counter = SERVO_START;
	
	DDRB &= ~(1 << sBIT_0); PORTB |= (1 << sBIT_0);
	DDRB &= ~(1 << sBIT_1); PORTB |= (1 << sBIT_1);
	DDRB &= ~(1 << sBIT_2); PORTB |= (1 << sBIT_2);
	
	DDRD |= (1 << PIN_SERVO);                               // dedicated PWM pin for timer 3 as output
	PORTD |= (1 << PIN_SERVO);                              // this must be done to disable OC4B, the pin is shared
	ICR3 = SERVO_WIDTH;                                     // the "TOP", makes the time period, OCR3A makes duty cycle
	TCCR3A |= (1 << COM3B1) | (1 << WGM31);                 // non inverting, mode14
	TCCR3B |= (1 << WGM33) | (1 << WGM32) | (1 << CS31);    // mode 14, prescale 8
	
	while(1)
	{
		OCR3B = servo_counter;
		if (servo_counter > SERVO_STOP ||
			servo_counter < SERVO_START )
		{
			servo_counter = SERVO_START;
			_delay_ms(SERVO_RETURN);
		}
		if (PINB & 0x01)
		{
			servo_counter = SERVO_001;
			_delay_ms(SERVO_HANG);
		}
		if (PINB & 0x02)
		{
			servo_counter = SERVO_010;
			_delay_ms(SERVO_HANG);
		}
		if (PINB & 0x03)
		{
			servo_counter = SERVO_011;
			_delay_ms(SERVO_HANG);
		}
		if (PINB & 0x04)
		{
			servo_counter = SERVO_100;
			_delay_ms(SERVO_HANG);
		}
		if (PINB & 0x05)
		{
			servo_counter = SERVO_101;
			_delay_ms(SERVO_HANG);
		}
		if (PINB & 0x06)
		{
			servo_counter = SERVO_110;
			_delay_ms(SERVO_HANG);
		}
		if (PINB & 0x07)
		{
			servo_counter = SERVO_111;
			_delay_ms(SERVO_HANG);
		}
		_delay_ms(SERVO_RETURN);
	}
	return EXIT_FAILURE;
}


////////~~~~~~~~END>  main.c
