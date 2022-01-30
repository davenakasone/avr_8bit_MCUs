#include <Keypad.h>
const int led_pin = 13;
const byte numRows= 4;
const byte numCols= 4; 

char keymap[numRows][numCols]= 
{
{'1', '2', '3', 'A'}, 
{'4', '5', '6', 'B'}, 
{'7', '8', '9', 'C'},
{'*', '0', '#', '\n'}
};

byte rowPins[numRows] = {9,8,7,6};
byte colPins[numCols]= {5,4,3,2};


Keypad myKeypad= Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void setup()
{
  	pinMode(led_pin, OUTPUT);
	Serial.begin(9600);
}

void loop()
{
  	digitalWrite(led_pin, LOW);
	char keypressed = myKeypad.getKey();
	if (keypressed != NO_KEY)
	{
		Serial.print(keypressed);
      	digitalWrite(led_pin, HIGH);
	}
  	delay(1);
}