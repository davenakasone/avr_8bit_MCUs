const int wiper = 0;
const int buzzerPin = 9;
int trail = 0;

void setup()
{
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
    int pot_val = analogRead(wiper);
    int f = map(pot_val, 0, 1023, 300, 1000);
  	if (trail != pot_val)
    {
      Serial.println(pot_val);
    }
  	tone(buzzerPin, f);
  	trail = pot_val;
	delay(1000);
}