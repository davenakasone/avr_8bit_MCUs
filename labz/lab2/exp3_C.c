#include <avr/io.h>
#include <util/delay.h>
int main (void)
{
	DDRB = 0xFF; //set PORTB as an output for LED
	DDRC = 0x00; //set PORTC as an input PORT
	PORTC = 0xFF; //activate the pull-up resistor for PORTC
	int MODE_ = 0b00011000;
	
	while (1)
	{
		//check if pushbutton connected to PINC.0 is pressed
		if ((PINC & 0x01) == 0x01)
		{
			PORTB = (MODE_ >> ~2); // If pressed, shifts 1 bit to right
			
		}
		else
		{	PORTB = MODE_ << 1;
			//seq <<1 ; //if the button is not pressed, turn OFF LEDs
		}
        _delay_ms(1000); // wait 1 second
	}
	return 0;
}
