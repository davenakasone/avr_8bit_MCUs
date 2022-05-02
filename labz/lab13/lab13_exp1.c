/*
	lab13, exp1a + buzzer
	displays buttons pressed
	LCD.rs  -->  PC0
	LCD.rw  -->  PC1
	LCD.en  -->  PC2
	LCD.[d7:d0]  -->  PD[7:0]
	
    LED -->  PB6  (or buzzer)
	
	ultrasonic
	Vcc     -> MCU "5V"
	gnd     -> MCU "gnd"
	echo    -> PB0
	trigger -> PB7
	
	rs = 0, commands
	rs = 1, data outputs
	rw = 0, write
	rw = 1, read
	en = 0, no latch
	en = 1, latch, needs 1 -> 0
	
	HC-SR04 ultra-sonic sensor
	PB7 for trigger
	PB0 for echo, must be use since timer1 has ICP1 on this pin
	sound will probably at 344 m/s where you use this
	for each tik, (1 / f_cpu) * prescale = T
	the sound travels to the target and back
	distance = (tiks / 2) * 344 meters, about    (1/8e6)*(344*100)/2 = 1860  , the const for distance in cm, must tune it
	"distance = rate * time ... m = m/s * s"
 */ 
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
#define LED_PIN 6        // there is an LED on PB6
#define ECHO_PIN 0       // on PORTB, must be on PB0 for ICR1
#define TRIGGER_PIN 7    // on PORTB
#define data PORTD

#define PUSH_WAIT_MS 2000
#define BUF_SIZ 16
#define TRIGGER_WIDTH_US 10    // duration of the trigger pulse in micro seconds
#define TIME_OUT 10            // overflows allowed to occur before continuing program

char helper[BUF_SIZ];
volatile int timer_overflows;
volatile long tik_count;
volatile long distance;

void lcd_init(void);             // initialize the LCD
void lcd_cmd(char cmd_out);      // send a command to the LCD
void lcd_data(char data_out);    // send char to LCD
void lcd_str(char* str);         // send a string to the LCD, char by char


int main(void)
{
	memset(helper, '\0', BUF_SIZ);
	distance = 0;
	tik_count = 0;
	timer_overflows = 0;
	
	DDRB |= (1 << LED_PIN);
	PORTB &= ~(1 << LED_PIN);
	
	DDRD = 0xFF;                                 // PB[7:0] output
	DDRC = (1 << rs) | (1 << rw) | (1 << en);    // outputs on PORTC
	
	DDRB |= (1 << TRIGGER_PIN);    // trigger signal as output
	PORTB |= (1 << ECHO_PIN);      // turn on pull up resistor for echo capture as input
	TIMSK1 |= (1 << TOIE1);        // enable timer1 overflow interrupt
	TCCR1A = 0;		               // normal operation
	
	lcd_init();
	sei();
	
    while (1) 
    {
		read_distance();
		
		snprintf(helper, BUF_SIZ-1, "cm:  %d", distance);
		if (distance < 10)
		{
			PORTB |= (1 << LED_PIN);
		}
		else
		{
			PORTB &= ~(1 << LED_PIN);
		}
		
		lcd_str(helper);
		_delay_ms(PUSH_WAIT_MS);
		lcd_cmd(0x1);
		_delay_ms(PUSH_WAIT_MS);
		
    }
	return EXIT_FAILURE;
}


////~~~~


void read_distance()
{
	memset(helper, '\0', BUF_SIZ);
	timer_overflows = 0;
	distance = 0;
	tik_count = 0;
	PORTB |= (1 << TRIGGER_PIN);                        // pulse begin
	_delay_ms(TRIGGER_WIDTH_US);                        // pulse high, duration in us
	PORTB &= ~(1 << TRIGGER_PIN);                       // pulse end
	TCNT1 = 0;                                          // clear timer1 counter
	TCCR1B = (1 << ICES1) | (1 << CS10);                // capture rising edge, no prescaling
	if (TIFR1 & (1 << ICF1)) {TIFR1 |= (1 << ICF1);}    // clear input capture flag
	timer_overflows = 0;                                // overflows reset
	while ((TIFR1 & (1 << ICF1)) == 0)                  // echo begins -> wait for rising edge
	{
		if (timer_overflows > TIME_OUT) {break;}        // time out causes break
	}
	timer_overflows = 0;                                // echo high, timer_overflows must reset
	TCNT1 = 0;                                          // clear timer1 counter
	TCCR1B = (1 << CS10);                               // falling edge capture, no prescaling
	if (TIFR1 & (1 << ICF1)) {TIFR1 |= (1 << ICF1);}    // clear input capture flag
	while ((TIFR1 & (1 << ICF1)) == 0)                  // echo ends -> wait for falling edge
	{
		if (timer_overflows > TIME_OUT) {break;}        // time out causes break
	}
	if (timer_overflows < TIME_OUT)                     // a valid measurement occurred
	{
		tik_count = ICR1 + 65535 * timer_overflows;
		distance =  tik_count/500;
	}
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


ISR(TIMER1_OVF_vect)
{
	timer_overflows++;
	if (timer_overflows > TIME_OUT*TIME_OUT ||
	timer_overflows < 0                  )
	{
		timer_overflows = 0;
	}
}


////////~~~~~~~~~END>  main.c