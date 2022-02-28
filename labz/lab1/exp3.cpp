/*
    lab1, exp3
    switch 4 LEDs
*/

#define LED0 13
#define LED1 12
#define LED2 11
#define LED3 10
int g_pos = 0;

void setup()
{
    pinMode(LED0, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
}

void loop()
{
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    delay(1);
    /*
    if (g_pos == 0)
    {
        digitalWrite(LED0, HIGH);
    }
    else if (g_pos == 1)
    {
        digitalWrite(LED1, HIGH);
    }
    else if (g_pos == 2)
    {
        digitalWrite(LED2, HIGH);
    }
    else
    {
        digitalWrite(LED3, HIGH);
    }
    */
        switch(g_pos)
        {
            case 0: digitalWrite(LED0, HIGH); break;
        case 1: digitalWrite(LED1, HIGH); break;
        case 2: digitalWrite(LED2, HIGH); break;
        case 3: digitalWrite(LED3, HIGH); break;
            default: break;
        }
    
    delay(1000);
    g_pos++;
    g_pos = g_pos % 4;
}

////////~~~~~~~~END>  