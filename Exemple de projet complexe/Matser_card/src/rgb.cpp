#include <arduino.h>

// Config RGB 3PINS
/*Ce code n'est pas utile dans le cas de la carte 2.0 
où se trouve une WS2812*/
/*Pour cette dernière on utilise "Freenove_WS2812_Lib_for_ESP32.h"*/

void rgb_setup(int r, int g ,int b){
      // RGB
  #if defined(ARDUINO_ARCH_ESP32) // ESP32 pinMode
    // assign rgb pins to channels
    ledcAttachPin(r,1);
    ledcAttachPin(g,2);
    ledcAttachPin(b,3);

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
void rgb_write(int rv, int gv ,int bv){
    ledcWrite(1,rv);
    ledcWrite(2,gv);
    ledcWrite(3,bv);
}
