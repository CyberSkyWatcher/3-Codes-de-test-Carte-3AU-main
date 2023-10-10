#include <Arduino.h>
/*test de base du module de gestion de thermocouple de type k*/
#include <SPI.h>
#include <Adafruit_MAX31855.h>


#define MAXCS   27

// Initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCS);


void setup() {
  Serial.begin(9600);

  Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
   Serial.print("C = "); 
   Serial.println(thermocouple.readCelsius());
   Serial.print("F = ");
   Serial.println(thermocouple.readFahrenheit());
 
   // For the MAX6675 to update, you must delay AT LEAST 250ms between reads!
   delay(1000);
}
