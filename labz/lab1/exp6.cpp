const int b_0 = 5;
const int b_1 = 8;
const int b_2 = 7;
const int b_3 = 6;
void control_7(int bcd);

void setup()
{
  pinMode(b_0,OUTPUT);
  pinMode(b_1,OUTPUT);
  pinMode(b_2,OUTPUT);
  pinMode(b_3,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  	int wiper = analogRead(A0);
  	Serial.print(wiper);
  	int digit = map(wiper,0,1023,0,9);
  	Serial.print("  ");
  	Serial.print(digit);
  	control_7(digit);
  	delay(1000);  
}

void control_7(int bcd)
{
  	digitalWrite(b_0, LOW);
  	digitalWrite(b_1, LOW);
  	digitalWrite(b_2, LOW);
  	digitalWrite(b_3, LOW);
  
	int bit_0 = bitRead(bcd, 0);
  	int bit_1 = bitRead(bcd, 1);
  	int bit_2 = bitRead(bcd, 2);
  	int bit_3 = bitRead(bcd, 3);
  
  	Serial.print("   ");
  	Serial.print(bit_3);
    Serial.print(" ");
  	Serial.print(bit_2);
  	Serial.print(" ");
  	Serial.print(bit_1);
  	Serial.print(" ");
  	Serial.print(bit_0);
  	Serial.print("\n\n");
  
  	if(bit_0 == 1) digitalWrite(b_0, HIGH);
  	if(bit_1 == 1) digitalWrite(b_1, HIGH);
  	if(bit_2 == 1) digitalWrite(b_2, HIGH);
  	if(bit_3 == 1) digitalWrite(b_3, HIGH);
}