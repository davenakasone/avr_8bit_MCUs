/*
	cpe310L, lab 11, exp 2a
	stepper turns CW, then CCW

	RX         --> PD0    // UART for viewing
	TX         --> PD1    // UART for viewing
	in1a       --> PB0    // stepper coil
	in1b       --> PB1    // stepper coil
	in2a       --> PB2    // stepper coil
	in2b       --> PB3    // stepper coil

	preparations >>>
		[hammer] -> toolchain -> AVR/GNU Linker -> General -> check "Use vprintf library(-Wl, -u, vprintf)
		[hammer] -> toolchain -> AVR/GNU Linker -> Miscellaneous -> Other Linker Flags -> "-lprintf_flt"
		tools -> Data Visualizer -> Configuration -> External Connection -> Serial Port -> 
			set the terminal's BAUD to 9600, open a terminal, add /r/n, COM3 "mEDBG"
			
	control the steps,
	
	1a | 1b | 2a | 2b
	_________________
	1  | 0  | 0  | 0
	0  | 1  | 0  | 0
	0  | 0  | 1  | 0
	0  | 0  | 0  | 1
	...
*/
#define F_CPU 16000000UL

#define STEP_1 0x8
#define STEP_2 0x4
#define STEP_3 0x2
#define STEP_4 0x1
#define PINS_STEP 0xF   // the stepper coil pins on PB[0:3]


#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/atomic.h>
#include <util/delay.h>

uint16_t step_CCW = 2000;
uint16_t step_CW = 6000;
uint16_t reps_CCW = 600;
uint16_t reps_CW = 200;
void make_delay_us(uint16_t stepper_pause_us);


int main(void)
{
	uint16_t repper = 0;
	DDRB = PINS_STEP;    // the stepper coil pins as output
	PORTB = 0;           // motor begins turned off
	
	while(1)
	{
		repper = reps_CCW;
		while (repper > 0)
		{
			PORTB = STEP_1;
			make_delay_us(step_CCW);
			PORTB = STEP_2;
			make_delay_us(step_CCW);
			PORTB = STEP_3;
			make_delay_us(step_CCW);
			PORTB = STEP_4;
			make_delay_us(step_CCW);
			repper--;
		}
		repper = reps_CW;
		_delay_ms(500);
		while (repper > 0)
		{
			PORTB = STEP_4;
			make_delay_us(step_CW);
			PORTB = STEP_3;
			make_delay_us(step_CW);
			PORTB = STEP_2;
			make_delay_us(step_CW);
			PORTB = STEP_1;
			make_delay_us(step_CW);
			repper--;
		}
	}
	return EXIT_FAILURE;
}


////~~~~


void make_delay_us(uint16_t stepper_pause_us)
{
	uint16_t counter = 0;
	while (counter < stepper_pause_us)
	{
		_delay_us(1);
		counter++;
	}
}


////////~~~~~~~~END>  main.c
