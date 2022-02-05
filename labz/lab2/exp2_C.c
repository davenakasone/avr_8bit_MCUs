#include <avr/io.h>

int main (void)
{
    DDRB = 0xFF; //set PORTB as an output for LED
    DDRC = 0x00; //set PORTC as an input PORT
    PORTC = 0xFF; //activate the pull-up resistor for PORTC
    
    while (1)
    {
        //check if pushbutton connected to PINC.0 is pressed
        if ((PINC & 0x01) == 0x01) 
        {
            PORTB = 0xFF; //if the button is pressed, turn ON LEDs
        }     
        else
        {
            PORTB = 0x00; //if the button is not pressed, turn OFF LEDs
        }
    }

    return 0;
}
