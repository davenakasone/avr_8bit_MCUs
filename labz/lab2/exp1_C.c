#include <avr/io.h>

int main (void)
{
    DDRB = 0xFF;    //set all of portB to output

    while (1)
    {
        PORTB = 0x88;    //set PB7 and PB3 high
    }

    return 0;
}
