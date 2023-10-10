#include <Arduino.h>
#include<IoAbstraction.h>
#include <TaskManagerIO.h>

// The pin onto which we connected the rotary encoders switch
const int spinwheelClickPin = 35;


// The two pins where we connected the A and B pins of the encoder. I recomend you dont change these
// as the pin must support interrupts.
const int encoderAPin = 34;
const int encoderBPin = 32;

// the maximum (0 based) value that we want the encoder to represent.
const int maximumEncoderValue = 128;

//
// When the spinwheel is clicked, this function will be run as we registered it as a callback
//
void onSpinwheelClicked(pinid_t pin, bool heldDown) {
  Serial.print("Button pressed ");
  Serial.println(heldDown ? "Held" : "Pressed");
}


//
// Each time the encoder value changes, this function runs, as we registered it as a callback
//
void onEncoderChange(int newValue) {
  Serial.print("Encoder change ");
  Serial.println(newValue);
}

void setup() {

  Serial.begin(9600);
  /*initialise les switchs*/
  /*true --> pul up || false --> pull down*/
  switches.initialise(ioUsingArduino(), false);

/*déclaration des switchs*/
  switches.addSwitch(spinwheelClickPin, onSpinwheelClicked);

  // now we set up the rotary encoder, first we give the A pin and the B pin.
  // we give the encoder a max value of 128, always minumum of 0.
  setupRotaryEncoderWithInterrupt(encoderAPin, encoderBPin, onEncoderChange);
  switches.changeEncoderPrecision(maximumEncoderValue, 0);
}

void loop() {
  taskManager.runLoop();  
}