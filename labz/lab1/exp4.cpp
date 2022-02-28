/*
	lab1, exp 4
*/

#define LED0 13
#define LED1 12
#define LED2 11
#define LED3 10
#define P_BUTi 8
#define P_BUTd 9

int g_pos = 0;
int pressed = 0;
void off_led();
void set_led();

void setup()
{
 	pinMode(LED0, OUTPUT);
  	pinMode(LED1, OUTPUT);
  	pinMode(LED2, OUTPUT);
  	pinMode(LED3, OUTPUT);
  	pinMode(P_BUTi, INPUT);
  	pinMode(P_BUTd, INPUT);
  	Serial.begin(9600);
}

void loop()
{
	off_led();
  	
  	pressed = digitalRead(P_BUTi);
  	Serial.print(pressed);
  	if(pressed == HIGH);
  	{
    	Serial.println(" : button {inc} pressed");
    	g_pos = (g_pos==3)? 0 : g_pos++;
  	}
  	pressed = digitalRead(P_BUTd);
  	Serial.print(pressed);
  	if (pressed == HIGH);
  	{
    	Serial.println(" : button {dec} pressed");
    	g_pos = (g_pos==0)? 3 : g_pos--;
  	}
  	Serial.print("\n");
  	set_led();
  	//g_pos = g_pos % 4; don't need this
  	delay(1000);
}


////~~~~


void off_led()
{
  	digitalWrite(LED0, LOW);
	digitalWrite(LED1, LOW);
  	digitalWrite(LED2, LOW);
  	digitalWrite(LED3, LOW);
  	delay(1);
}


////~~~~


void set_led()
{
    switch(g_pos)
    {
        case 0: digitalWrite(LED0, HIGH); break;
        case 1: digitalWrite(LED1, HIGH); break;
        case 2: digitalWrite(LED2, HIGH); break;
        case 3: digitalWrite(LED3, HIGH); break;
        default: break;
    }
}

////////~~~~~~~~END
