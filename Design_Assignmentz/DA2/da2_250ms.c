/*
	da2, t1, demonstrates a 250ms delay
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRB = 1<<5;    // PORTB.5 as output
	PORTB = 0;      // turn LEDs off
	
    while (1) 
    {
		PORTB = (1 << 5);    // turn LED on
		_delay_ms(250);
		PORTB = 0;           // turn LED off
		_delay_ms(250);
    }
}

////////~~~~~~~END>  main.c
