/*
	lab 5, exp3
	a PWM signal 3 seconds long is made
	
	INT0 events    Duty cycle
	-------------------------
	    0       |     0          the LED is off
		1       |     0.1        the LED is on 10% of the time
		2		|     0.2        ...
		3       |     0.3
		4       |     0.4
		5       |     0.5
		6		|	  0.6
		7       |     0.7
		8       |     0.8
		9       |     0.9        the LED is on 90% of the time
		10      |     1          the LED is on 
	
	the INT0 counter resets after 10 events
	{PD.3} has INT1
	PB.5 used as verification, LED
*/ 

#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

const uint16_t INT_PIN = 3;
const uint16_t MY_LED = 5;
const uint16_t INT_IND = 0;
volatile uint8_t int1_counter = 0;    // global counter for INT1 events


int main
(
	void
)
{
    DDRB = (1 << MY_LED) | (1 << INT_IND);      // configure PORTB as output for LEDs
	PORTB = 0;                                  // start with LEDs off
	DDRD = 0;                                   // PORTD as input
	PORTD = (1 << INT_PIN);                     // turn on pull-up resistor
	EIMSK = (1 << INT1);                        // enable INT1 external interrupt
	EICRA = (1 << ISC11);                       // make INT1 falling-edge triggered
	
	volatile uint16_t on_time = 0;     // find duty cycle D
	volatile uint16_t off_time = 0;    // find remainder of T, 1 - D
	volatile uint8_t pass = 0;         // stores global counter
	sei();                             // enable interrupts

    while (1) 
	{
		pass = int1_counter;
		//
		switch(pass)
		{
			case 1 :
				on_time = 300;
				off_time = 2700;
				break;
			case 2 :
				on_time = 600;
				off_time = 2400;
				break;
			case 3 :
				on_time = 900;
				off_time = 2100;
				break;
			case 4 :
				on_time = 1200;
				off_time = 1800;
				break;
			case 5 :
				on_time = 1500;
				off_time = 1500;
				break;
			case 6 :
				on_time = 1800;
				off_time = 1200;
				break;
			case 7 :
				on_time = 2100;
				off_time = 900;
				break;
			case 8 :
				on_time = 2400;
				off_time = 600;
				break;
			case 9 :
				on_time = 2700;
				off_time = 300;
				break;
			case 10 :
				on_time = 2900;
				off_time = 10;
				break;
			default :
				on_time = 10;
				off_time = 2990;
		}
		//
		//on_time = (pass/10) * 3000;
		//off_time = 3000 - pass;
		PORTB = (1 << MY_LED);
		while(on_time > 0)
		{
			_delay_ms(1);
			on_time = on_time - 1;
		}
		PORTB = 0;
		while(off_time > 0)
		{
			_delay_ms(1);
			off_time = off_time - 1;
		}
	}    

	return EXIT_FAILURE;
}


////~~~~


ISR(INT1_vect)    
{
	PORTB |= (1 << INT_IND); 
	_delay_ms(10);
	PORTB &= ~(1 << INT_IND);
	int1_counter++;
	if (int1_counter < 0 || int1_counter > 10)
	{
		int1_counter = 0;
	}
}                         


////////~~~~~~~~END>  main.c
