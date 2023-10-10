// test de la RGB et du POT
// A noter que sur l'ESP32 la fonction analogwrite() n'existe pas ... il faut passer par une autre fonction 

/* Ce code utilise une led RGB 3PIN --> la carte 3AU 2.0 propose une LED RGB WS2812 
qui s'utilise via une seule pin*/
/* Un code pour cette dernière est également disponible -- pensez à  vérifier quelle version vous avez*/

#include <Arduino.h>
//POT
#define POT 33
// RGB pins
#define GREEN_PIN 13
#define RED_PIN   16
#define BLUE_PIN  17
//

// var
int valpot;
int vitesse;

void setup() {
    Serial.begin(9600);
    pinMode(POT,INPUT);
    #if defined(ARDUINO_ARCH_ESP32) // ESP32 pinMode
        // assign rgb pins to channels
        ledcAttachPin(RED_PIN,1);
        ledcAttachPin(GREEN_PIN,2);
        ledcAttachPin(BLUE_PIN,3);

        // init. channels
        ledcSetup(1, 1000, 8);
        ledcSetup(2, 1000, 8);
        ledcSetup(3, 1000, 8);
    #else
        pinMode(RED_PIN, OUTPUT);
        pinMode(GREEN_PIN, OUTPUT);
        pinMode(BLUE_PIN, OUTPUT);
    #endif

}

void loop() {
// 4 couleurs pour faire simple
valpot = analogRead(POT);
vitesse = map(valpot,0,4095,10,10000);
    for(int i=0;i<=255;i+=10)
    {
        Serial.println(i);
        ledcWrite(1,i);
        for(int j=0; j<=255; j+=10)
        {
            ledcWrite(2,j);
            Serial.println(j);
            for(int k=0; k<=255; k+=10)
            {
                ledcWrite(3,k);
                Serial.println(k);
                delay(vitesse);
            }
        }
    }
    delay(60000);
}