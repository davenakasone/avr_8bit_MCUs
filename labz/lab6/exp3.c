/*
	lab6, exp3
	displays buttons pressed
	LCD.rs  -->  PC0
	LCD.rw  -->  PC1
	LCD.en  -->  PC2
	LCD.[d7:d0]  -->  PB[7:0]
	KeyCol[1:4] --> PD[0:3]
	KeyRow[1:4] --> PD[4:7]

	1  2  3  A
    4  5  6  B
    7  8  9  C
	*  0  #  D
    
	rs = 0, commands
	rs = 1, data outputs
	rw = 0, write
	rw = 1, read
	en = 0, no latch
	en = 1, latch, needs 1 -> 0
	
	combination is "123A"
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define rs PC0
#define rw PC1
#define en PC2
#define data PORTB
#define PUSH_WAIT_MS 1000
#define COMBO_WAIT_MS 10000
#define COMBO_SIZE 5

const char MY_COMBO[] = "123A";       // correct combo to check
uint8_t press_counter;                // counts button presses
char running_combo[COMBO_SIZE];       // 4 char + 1 NULL

void lcd_init(void);                     // initialize the LCD
void lcd_cmd(char cmd_out);              // send a command to the LCD
void lcd_data(char data_out);            // send char to LCD
void lcd_str(char* str);                 // send a string to the LCD, char by char
void check1(void);                       // check the input R/C, index 1
void check2(void);                       // check the input R/C, index 2
void check3(void);                       // check the input R/C, index 3
void check4(void);                       // check the input R/C, index 4
void check_combo(void);                  // called to check combination
void update_combo(const char* appnd);    // appends keypress to combination


int main(void)
{
	DDRD = 0x0F;                                 // PD[7:4] input, PD[3:0] output
	DDRB = 0xFF;                                 // PB[7:0] input
	DDRC = (1 << rs) | (1 << rw) | (1 << en);    // outputs on PORTC
	memset(running_combo, '\0' ,COMBO_SIZE);     // null the combo keeper
	press_counter = 0;                           
	lcd_init();

    while (1) 
    {
		// check the button press, delay to stop switch bounce
		check1();
		_delay_ms(15);
		check2();
		_delay_ms(15);
		check3();
		_delay_ms(15);
		check4();
		_delay_ms(15);
		check_combo();
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


void check1(void)
{
	PORTD = 0b00000001;    // set output on PD0 high
	if ((PIND & 0b11110000) == 0b00010000)         // PD0 & PD4
	{
		lcd_str("press[1,4]  A");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1); 
		update_combo("A");
	}
	else if ((PIND & 0b11110000) == 0b00100000)    // PD0 & PD5
	{
		lcd_str("press[2,1]  4");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("4");
	}
	else if ((PIND & 0b11110000) == 0b01000000)    // PD0 & PD6
	{
		lcd_str("press[3,1]  7");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("7");
	}
	else if ((PIND & 0b11110000) == 0b10000000)    // PD0 & PD7
	{
		lcd_str("press[4,1]  *");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("*");
	}
	else
	{
		lcd_cmd(0xC); // display on cursor off
	}
}


////~~~~


void check2(void)
{
	PORTD = 0b00000010;    // set output on PD1 high
	if ((PIND & 0b11110000) == 0b00010000)         // PD1 & PD4
	{
		lcd_str("press[1,1]  1");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("1");
	}
	else if ((PIND & 0b11110000) == 0b00100000)    // PD1 & PD5
	{
		lcd_str("press[2,2]  5");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("5");
	}
	else if ((PIND & 0b11110000) == 0b01000000)    // PD1 & PD6
	{
		lcd_str("press[3,2]  8");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("8");
	}
	else if ((PIND & 0b11110000) == 0b10000000)    // PD1 & PD7
	{
		lcd_str("press[4,2]  0");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("0");
	}
	else
	{
		lcd_cmd(0xC); // display on cursor off
	}
}


////~~~~

void check3(void)
{
	PORTD = 0b00000100;    // set output on PD2 high
	if ((PIND & 0b11110000) == 0b00010000)         // PD2 & PD4
	{
		lcd_str("press[1,2]  2");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("2");
	}
	else if ((PIND & 0b11110000) == 0b00100000)    // PD2 & PD5
	{
		lcd_str("press[2,3]  6");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("6");
	}
	else if ((PIND & 0b11110000) == 0b01000000)    // PD2 & PD6
	{
		lcd_str("press[3,3]  9");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("9");
	}
	else if ((PIND & 0b11110000) == 0b10000000)    // PD2 & PD7
	{
		lcd_str("press[4,3]  #");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("#");
	}
	else
	{
		lcd_cmd(0xC); // display on cursor off
	}
}


////~~~~


void check4(void)
{
	PORTD = 0b00001000;    // set output on PD3 high
	if ((PIND & 0b11110000) == 0b00010000)         // PD3 & PD4
	{
		lcd_str("press[1,3]  3");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("3");
	}
	else if ((PIND & 0b11110000) == 0b00100000)    // PD3 & PD5
	{
		lcd_str("press[2,4]  B");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("B");
	}
	else if ((PIND & 0b11110000) == 0b01000000)    // PD3 & PD6
	{
		lcd_str("press[3,4]  C");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("C");
	}
	else if ((PIND & 0b11110000) == 0b10000000)    // PD3 & PD7
	{
		lcd_str("press[4,4]  D");
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		update_combo("D");
	}
	else
	{
		lcd_cmd(0xC); // display on cursor off
	}
}


/////~~~~


void check_combo(void)
{
	// wait for 4 key presses
	if (press_counter < COMBO_SIZE-1) 
	{ 
		return;
	}
	else
	{
		int result = strcmp(running_combo, MY_COMBO);
		if (result == 0) // combo is correct
		{
			lcd_str("Lock Opened");
			_delay_ms(COMBO_WAIT_MS);
			lcd_cmd(0x1);
		}
		else // the combo is wrong
		{
			lcd_str("wrong");
			_delay_ms(COMBO_WAIT_MS);
			lcd_cmd(0x1);
		}
		press_counter = 0;                          // reset key presses
		memset(running_combo, '\0', COMBO_SIZE);    // resets combination
	}
}


////~~~~


void update_combo(const char* appnd)
{
	strcat(running_combo, appnd); // append char to combination
	press_counter++;              // count another entry
}

////////~~~~~~~~~END>  main.c
