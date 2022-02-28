/*
	lab1, exp1
    alternates LEDs  on/off
    #include <Arduino.h> // already done
    https://www.tinkercad.com/things/du3xcP9zcy1-exp1
*/


void setup()
{
  pinMode(2, OUTPUT);
  pinMode(8, OUTPUT);
}


////~~~~


void loop()
{
  digitalWrite(8, HIGH);
  delay(1000);
  digitalWrite(8, LOW);

  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
}


////////~~~~~~~~END>
