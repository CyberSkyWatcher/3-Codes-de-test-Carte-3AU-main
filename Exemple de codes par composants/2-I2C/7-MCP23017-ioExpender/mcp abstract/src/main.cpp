#include <Arduino.h>
#include <Wire.h>
#include <IoAbstraction.h>
#include <IoAbstractionWire.h>
#include<TaskManagerIO.h>
#include <KeyboardManager.h>

#define relais 25

// Keypad I/O with MCP23017
#define ROW1 8  // GPB0
#define ROW2 9  // GPB1
#define ROW3 10 // GPB2
#define ROW4 11 // GPB3
#define COL1 12 // GPB4
#define COL2 13 // GPB5
#define COL3 14 // GPB6
#define COL4 15 // GPB7

MAKE_KEYBOARD_LAYOUT_4X4(keyLayout)

//keyboard manager class
MatrixKeyboardManager keyboard;

IoAbstractionRef ioExpander = ioFrom23017(0x20);

// We need a class that extends from KeyboardListener. this gets notified when
// there are changes in the keyboard state.

int cpt;
char code[4];
bool flag;


void seg_nb(int indice){
  if (code[indice]=='0'){
    ioDeviceDigitalWrite(ioExpander,0,LOW);
    ioDeviceDigitalWrite(ioExpander,1,LOW);
    ioDeviceDigitalWrite(ioExpander,2,LOW);
    ioDeviceDigitalWrite(ioExpander,3,LOW);
  }
  if (code[indice]=='1'){
    ioDeviceDigitalWrite(ioExpander,0,HIGH);
    ioDeviceDigitalWrite(ioExpander,1,LOW);
    ioDeviceDigitalWrite(ioExpander,2,LOW);
    ioDeviceDigitalWrite(ioExpander,3,LOW);
  }
  if (code[indice]=='2'){
    ioDeviceDigitalWrite(ioExpander,0,LOW);
    ioDeviceDigitalWrite(ioExpander,1,HIGH);
    ioDeviceDigitalWrite(ioExpander,2,LOW);
    ioDeviceDigitalWrite(ioExpander,3,LOW);
  }
  if (code[indice]=='3'){
    ioDeviceDigitalWrite(ioExpander,0,HIGH);
    ioDeviceDigitalWrite(ioExpander,1,HIGH);
    ioDeviceDigitalWrite(ioExpander,2,LOW);
    ioDeviceDigitalWrite(ioExpander,3,LOW);
  }
  if (code[indice]=='4'){
    ioDeviceDigitalWrite(ioExpander,0,LOW);
    ioDeviceDigitalWrite(ioExpander,1,LOW);
    ioDeviceDigitalWrite(ioExpander,2,HIGH);
    ioDeviceDigitalWrite(ioExpander,3,LOW);
  }
  if (code[indice]=='5'){
    ioDeviceDigitalWrite(ioExpander,0,HIGH);
    ioDeviceDigitalWrite(ioExpander,1,LOW);
    ioDeviceDigitalWrite(ioExpander,2,HIGH);
    ioDeviceDigitalWrite(ioExpander,3,LOW);
  }
  if (code[indice]=='6'){
    ioDeviceDigitalWrite(ioExpander,0,LOW);
    ioDeviceDigitalWrite(ioExpander,1,HIGH);
    ioDeviceDigitalWrite(ioExpander,2,HIGH);
    ioDeviceDigitalWrite(ioExpander,3,LOW);
  }
  if (code[indice]=='7'){
    ioDeviceDigitalWrite(ioExpander,0,HIGH);
    ioDeviceDigitalWrite(ioExpander,1,HIGH);
    ioDeviceDigitalWrite(ioExpander,2,HIGH);
    ioDeviceDigitalWrite(ioExpander,3,LOW);
  }
  if (code[indice]=='8'){
    ioDeviceDigitalWrite(ioExpander,0,LOW);
    ioDeviceDigitalWrite(ioExpander,1,LOW);
    ioDeviceDigitalWrite(ioExpander,2,LOW);
    ioDeviceDigitalWrite(ioExpander,3,HIGH);
  }
  if (code[indice]=='9'){
    ioDeviceDigitalWrite(ioExpander,0,HIGH);
    ioDeviceDigitalWrite(ioExpander,1,LOW);
    ioDeviceDigitalWrite(ioExpander,2,LOW);
    ioDeviceDigitalWrite(ioExpander,3,HIGH);
  }
  if (code[indice]=='A'){
    ioDeviceDigitalWrite(ioExpander,0,LOW);
    ioDeviceDigitalWrite(ioExpander,1,HIGH);
    ioDeviceDigitalWrite(ioExpander,2,LOW);
    ioDeviceDigitalWrite(ioExpander,3,HIGH);
  }
  if (code[indice]=='B'){
    ioDeviceDigitalWrite(ioExpander,0,HIGH);
    ioDeviceDigitalWrite(ioExpander,1,HIGH);
    ioDeviceDigitalWrite(ioExpander,2,LOW);
    ioDeviceDigitalWrite(ioExpander,3,HIGH);
  }
  if (code[indice]=='C'){
    ioDeviceDigitalWrite(ioExpander,0,LOW);
    ioDeviceDigitalWrite(ioExpander,1,LOW);
    ioDeviceDigitalWrite(ioExpander,2,HIGH);
    ioDeviceDigitalWrite(ioExpander,3,HIGH);
  }
  if (code[indice]=='D'){
    ioDeviceDigitalWrite(ioExpander,0,HIGH);
    ioDeviceDigitalWrite(ioExpander,1,LOW);
    ioDeviceDigitalWrite(ioExpander,2,HIGH);
    ioDeviceDigitalWrite(ioExpander,3,HIGH);
  }
}

void seg_write(int digit){
  //Serial.print(digit);
  if (digit==0){
      ioDeviceDigitalWrite(ioExpander,4,LOW);
      ioDeviceDigitalWrite(ioExpander,5,LOW);
      ioDeviceDigitalWrite(ioExpander,6,LOW);
      ioDeviceDigitalWrite(ioExpander,7,HIGH);
  }
  if (digit==1){
      ioDeviceDigitalWrite(ioExpander,4,LOW);
      ioDeviceDigitalWrite(ioExpander,5,LOW);
      ioDeviceDigitalWrite(ioExpander,6,HIGH);
      ioDeviceDigitalWrite(ioExpander,7,LOW);
  }
  if (digit==2){
      ioDeviceDigitalWrite(ioExpander,4,LOW);
      ioDeviceDigitalWrite(ioExpander,5,HIGH);
      ioDeviceDigitalWrite(ioExpander,6,LOW);
      ioDeviceDigitalWrite(ioExpander,7,LOW);
  }
  if (digit==3){
      ioDeviceDigitalWrite(ioExpander,4,HIGH);
      ioDeviceDigitalWrite(ioExpander,5,LOW);
      ioDeviceDigitalWrite(ioExpander,6,LOW);
      ioDeviceDigitalWrite(ioExpander,7,LOW);
  }
  seg_nb(digit);
}



class MyKeyboardListener : public KeyboardListener {
public:
    void keyPressed(char key, bool held) override {
        Serial.print("Key ");
        Serial.print(key);
        Serial.print(" is pressed, held = ");
        Serial.println(held);
        code[cpt] = key;
        seg_write(cpt);
        cpt++;
    }

    void keyReleased(char key) override {
        Serial.print("Released ");
        Serial.println(key);
        if (cpt > 3){
          flag = 1;
          cpt = 0;
        }
    }
} myListener;



void setup() {
    Wire.begin(); //function for initialise Wire (I2C)
    Serial.begin(9600);
    delay(1000);

    pinMode(relais,OUTPUT);
    for(int k = 0;k<8;k++){
      ioDevicePinMode(ioExpander, k, OUTPUT);
    }

    keyLayout.setRowPin(0, ROW1);
    keyLayout.setRowPin(1, ROW2);
    keyLayout.setRowPin(2, ROW3);
    keyLayout.setRowPin(3, ROW4);
    keyLayout.setColPin(0, COL1);
    keyLayout.setColPin(1, COL2);
    keyLayout.setColPin(2, COL3);
    keyLayout.setColPin(3, COL4);

    // create the keyboard mapped to arduino pins and with the layout chosen above.
    // it will callback our listener
    keyboard.initialise(ioExpander, &keyLayout, &myListener);

    // start repeating at 850 millis then repeat every 350ms
    keyboard.setRepeatKeyMillis(850, 350);

    Serial.println("Keyboard is initialised!");
}

void loop() {
    // as this indirectly uses taskmanager, we must include this in loop.
    taskManager.runLoop();
    if (flag){
      cpt = 0;
      //Serial.println(code);
      cpt = millis() + 5000;
      while(cpt>millis()){
        for(int i = 0;i<=3;i++){
          seg_write(i);
          Serial.print(i);
          
          taskManager.runLoop();
          delay(100);
        }
      }
      Serial.print('\n');
      Serial.print("le code est : ");
      Serial.println(code);
      cpt = 0;
      flag = 0;
      if(code[0]=='3' && code[1] =='2' && code[2] == '1' && code[3] == '0'){
        Serial.println("code ok");
        digitalWrite(relais,HIGH);
        delay(2000);
        digitalWrite(relais,LOW);

        ioDeviceDigitalWrite(ioExpander,4,LOW);
        ioDeviceDigitalWrite(ioExpander,5,LOW);
        ioDeviceDigitalWrite(ioExpander,6,LOW);
        ioDeviceDigitalWrite(ioExpander,7,LOW);
      }
    }
}
