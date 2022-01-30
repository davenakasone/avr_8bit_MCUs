/*
	lab1 exp2
    observe range of unsigned char [255:0]
    https://www.tinkercad.com/things/gp534N1Ig2p-exp2
    #include <Arduino.h> // already done
*/


void setup()
{
	Serial.begin(9600);
}


////~~~~


void loop()
{
  
  for(unsigned char ii = 0; ii < 1000; ii++)
  {
    Serial.println(ii);
  }
}


/////////~~~~~~~~~~END
