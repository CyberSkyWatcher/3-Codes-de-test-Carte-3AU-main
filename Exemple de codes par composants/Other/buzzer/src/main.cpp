#include <Arduino.h>
#include "EasyBuzzer.h"


#define switch 25
#define buzzer 17

unsigned int frequency = 1000;
unsigned int beeps = 10;

void done()
{	
	delay(2000);
	digitalWrite(switch,HIGH);
	delay(2000);
	digitalWrite(switch,LOW);
	Serial.println("test");
}
void setup()
{
	Serial.begin(9600);

	pinMode(switch, OUTPUT); 
	Serial.print("Starting Buzzer...");
	EasyBuzzer.setPin(buzzer);
	EasyBuzzer.beep(frequency, beeps, done);
}

void loop()
{
	EasyBuzzer.update();

}