/*
	lab9, exp3
	writes raw ADC value to LCD
	ADC raw      -->  PC0
	LCD.rs       -->  PC1
	LCD.rw       -->  PC2
	LCD.en       -->  PC3
	LCD.[d7:d0]  -->  PB[7:0]
    
	rs = 0, commands
	rs = 1, data outputs
	rw = 0, write
	rw = 1, read
	en = 0, no latch
	en = 1, latch, needs 1 -> 0
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define adc 0
#define rs 1
#define rw 2
#define en 3
#define STR_SIZ 16

volatile uint16_t adc_val;         // collects ADC value
volatile char adc_str[STR_SIZ];    // string to hold adc value

void lcd_init(void);             // initialize the LCD
void lcd_cmd(char cmd_out);      // send a command to the LCD
void lcd_data(char data_out);    // send char to LCD
void lcd_str(char* str);         // send a string to the LCD, char by char
void read_adc (void);                   


int main(void)
{
	adc_val = 0;
	memset(adc_str, '\0', STR_SIZ);
	
	DDRB = 0xFF;                                 // PB[7:0] as output for data bits
	DDRC = (1 << rs) | (1 << rw) | (1 << en);    // controls as outputs on PORTC
	PORTC |= (1 << adc);                         // turn on ADC pullup
	                
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    // 16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (0<<REFS0);                             // Voltage reference from AREF
	ADCSRA |= (1<<ADEN);                             // turn on ADC
	ADCSRA |= (1<<ADSC);                             // get first conversion done
	          
	lcd_init();

    while (1) 
    {
		read_adc();
		snprintf(adc_str, STR_SIZ-1, "ADC:  %d", adc_val);
		lcd_str(adc_str);
		_delay_ms(2000);  // hold output for 2 seconds on LCD
		lcd_init();
		_delay_ms(500);  
    }
	return EXIT_FAILURE;
}


////~~~~


void lcd_init(void)
{
	lcd_cmd(0x38);   // 2-line, 5x7 font
	_delay_ms(9);
	lcd_cmd(0xE);    // display on, cursor on
	_delay_ms(9);
	lcd_cmd(0x1);    // clear the LCD
	_delay_ms(9);
	lcd_cmd(0x80);   // set cursor to top left
	_delay_ms(9);
}


////~~~~


void lcd_cmd(char cmd_out)
{
	data = cmd_out;
	PORTC &= ~(1 << rs);    // rs = cmd
	PORTC &= ~(1 << rw);    // rw = write
	PORTC |= (1 << en);     // enable
	_delay_ms(12);
	PORTC &= ~(1 << en);    // disable
	_delay_ms(12);
}


////~~~~


void lcd_data(char data_out)
{
	PORTB = data_out;
	PORTC |= (1 << rs);     // rs = data
	PORTC &= ~(1 << rw);    // rw = write
	PORTC |= (1 << en);     // enable
	_delay_ms(12);
	PORTC &= ~(1 << en);    // disable
	_delay_ms(12);
}


////~~~~


void lcd_str(char* str)
{
	unsigned int ii = 0;
	while (str[ii] != '\0')
	{
		lcd_data(str[ii]);  // send chars until end of string
		ii++;
	}
}


////~~~~


void read_adc (void)
{
	ADMUX &= 0xF0;                 // clears channels
	ADMUX |= adc;                  // set new channel to read
	ADCSRA |= (1 << ADSC);         // starts a new conversion
	while(ADCSRA & (1<<ADSC)){}    // wait until the conversion is done
	adc_val = ADCW;                // gets the ADC value
}


////////~~~~~~~~~END>  main.c
