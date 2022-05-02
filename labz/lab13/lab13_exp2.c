/*
	lab13, exp2
	displays buttons pressed
	LCD.rs  -->  PC0
	LCD.rw  -->  PC1
	LCD.en  -->  PC2
	LCD.[d7:d0]  -->  PD[7:0]
	
	PC4 --> ADC  (or temperature sensor)
	
	rs = 0, commands
	rs = 1, data outputs
	rw = 0, write
	rw = 1, read
	en = 0, no latch
	en = 1, latch, needs 1 -> 0
 */ 

#define CALC_TEMP 1917 // turn on to convert raw ADC into degrees celcius

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define rs 0
#define rw 1
#define en 2
#define adc 4 // ADC using channel 4 on PC4
#define data PORTD

#define BUF_SIZ 16
#define PUSH_WAIT_MS 2000

char helper[BUF_SIZ];
volatile uint16_t adc_val;         // collects ADC value

void lcd_init(void);             // initialize the LCD
void lcd_cmd(char cmd_out);      // send a command to the LCD
void lcd_data(char data_out);    // send char to LCD
void lcd_str(char* str);         // send a string to the LCD, char by char
void read_adc (void);  


int main(void)
{
	adc_val = 0;

	DDRD = 0xFF;                                 // PB[7:0] output
	DDRC = (1 << rs) | (1 << rw) | (1 << en);    // outputs on PORTC

	PORTC |= (1 << adc);                             // turn on ADC pullup
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));    // 16Mhz/128 = 125Khz the ADC reference clock
	ADMUX |= (0<<REFS0);                             // Voltage reference from AREF
	ADCSRA |= (1<<ADEN);                             // turn on ADC
	ADCSRA |= (1<<ADSC);                             // get first conversion done
	
	lcd_init();
	
    while (1) 
    {
		
		read_adc();
		#ifdef CALC_TEMP
			snprintf(helper, BUF_SIZ-1, "ADC:  %u", 5*(adc_val/1023)*416 - 273);
		#else
			snprintf(helper, BUF_SIZ-1, "ADC:  %u", adc_val);
		#endif
		
		lcd_str(helper);
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		_delay_ms(PUSH_WAIT_MS);
		
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
	data = data_out;
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
