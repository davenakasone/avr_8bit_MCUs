/*
	made for 328pb
		change to 328p, use portB
	led1 = PORTD0
	led2 = PORTD1
	led3 = PORTD2
	led4 = PORTD3
	switch0 = PORTC0
	switch1 = PORTC1
*/

#define F_CPU 16000000UL    // 16 MHz
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    DDRD = 0x0F; // portD 0, 1, 2, 3 as output
	DDRC = 0x00; // portB is input / buttons
	PORTC = 0xFF; // activate pull-up resistors
	int toggle = 0;
  
    while (1) 
    {
		if ((PINC & 0x03) == 0x03) // 11
		{
			PORTD = 0x03; // led 1 and 2 on, led 3 and 4 off
		}
		else if ((PINC & 0x01) == 0x01) // 01
		{
			if (toggle == 1)
			{
				PORTD = 0x0A; // led 1 and 3 on, led 2 and 4 off
				toggle = 0;
			}
			else
			{
				PORTD = 0x05; // led 2 and 4 on, led 1 and 3 off
				toggle = 1;	
			}
		}
		else if ((PINC & 0x03) == 0x02) // 10
		{
			PORTD = 0x00; // all off
		}
		else
		{
			PORTD = 0x0F; // all on
		}
		_delay_ms(1000);
    }
	return 0;
}
