/*
	cpe301, da2, all tasks
 */ 

#define F_CPU 16000000UL    // 16 MHz for 328pb

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define SW_POL 3          // switch on C2 triggers poll
#define SW_INT 2          // switch on D2 triggers interrupt
#define LED_POL 2         // LED on B2 indicates poll detected
#define LED_INT 5         // LED on B5 indicates interrupt detected
#define INIT_LEDS 0x3C    // turns reversed biased LEDs on shield off


int main(void)
{
	DDRB = 0xFF;             // PORTB as output
	PORTB = INIT_LEDS;       // turn LEDs off
	DDRC = 0;                // PORTC as input
	PORTC |= 1 << SW_POL;    // enable pullup resistor on C3
	DDRD = 0;                // PORTD as input
	PORTD |= 1 <<SW_INT;     // enable pullup resistor on D2
	EICRA = 0x2;             // INT0 on falling edge
	EIMSK = 1 << INT0;       // enable INT0
	sei();                   // enable interrupts
	
    while (1) 
    {
		
		if (!(PINC & 0x4)) // if C3 was pressed, poll triggered
		{
			PORTB &= ~(1 << LED_POL);    // turn on B2 LED
			_delay_ms(250*5);            // wait 1.25 seconds
			PORTB = INIT_LEDS;           // turn all the LEDS off
		}
    }
}


////~~~~


ISR (INT0_vect)    // isr for external interrupt 0
{
	PORTB &= ~(1 << LED_INT);    // turn on B5 LED
	_delay_ms(250*2);            // wait 0.5 seconds
	PORTB = INIT_LEDS;           // turn all the LEDS off
}


////////~~~~~~~END>  main.c
