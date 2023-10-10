/*Code d'exemple pour utilisation de la led ws2812*/
/* Ce code parcours toutes les couleurs de la led en 3 min*/
/* Le principe est identique à  celui  réalisé avec la led 3pin */ 
#include <Arduino.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"

/*ce genre de led se trouvent souvent sur des rubans de led*/
/*Dès lors la bibliothèque propose la gestion du ruban de leds "LED_count" indique le nb de leds sur le ruban*/

#define LEDS_COUNT   1			//nombre de leds
#define LEDS_PIN     0			
#define CHANNEL   	 0			// comme évoqué avec la led 3pins --> pas de analogW sur esp32

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

void setup() {
  strip.begin();                //strip -- ruban = initialisation de /des leds
  strip.setBrightness(50);      //Fixe la luminosité de la led (0-255)
}

void loop(){
/* LE code principal qui change la couleur de la led toutes les 300ms 
et qui en 3min passe toutes les couleurs*/
  for(int i=0;i<=255;i+=32)
  {
    Serial.println(i);
    strip.setLedColorData(0, i, 0, 0);
    strip.show();
    for(int j=0; j<=255; j+=32)
    {
      strip.setLedColorData(0, i, j, 0);
      strip.show();
      Serial.println(j);
      for(int k=0; k<=255; k+=32)
      {
        strip.setLedColorData(0, i, j, k);
        strip.show();
        Serial.println(k);
        delay(300);
      }
    }
  }
}