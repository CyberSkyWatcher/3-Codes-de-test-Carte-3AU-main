#include <Arduino.h>
#include "EasyBuzzer.h"
#include <DHT.h>

#define switch 25
#define buzzer 17

// dht type --> dht11
#define DHTPIN 26
#define DHTTYPE    DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);

unsigned int frequency = 1000;
unsigned int beeps = 1;

void done()
{  
  digitalWrite(switch,HIGH);
  delay(2000);
  digitalWrite(switch,LOW);
}
void setup()
{
  Serial.begin(9600);
  dht.begin();
  pinMode(switch, OUTPUT); 
  Serial.print("Starting Buzzer...");
  EasyBuzzer.setPin(buzzer);
  
}

void loop()
{
  EasyBuzzer.update();
   // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  Serial.println(t);
  Serial.println(h);

  if(t>=30 || h>=50){
    EasyBuzzer.beep(frequency, beeps, done);
  }
  else{
    EasyBuzzer.stopBeep();
  }
  delay(100);
}