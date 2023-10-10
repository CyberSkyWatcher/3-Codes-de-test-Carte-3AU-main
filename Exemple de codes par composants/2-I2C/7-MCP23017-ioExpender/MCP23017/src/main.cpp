#include <Arduino.h>

#include <Wire.h> 
#include "Adafruit_MCP23017.h"
#include "I2CScanner.h"

#define bptactile 14
#define bp1 0 
#define bp2 36
#define relais 25

Adafruit_MCP23017 mcp;
/* 
  Attention dans la lib il faut aller éditer la déclaration de l'adresse qui 
  pour une raison que j'ignore ne peut pas être supérieure à 7
  => i2C scanner pour trouver les adresses
*/
I2CScanner scanner;

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

uint8_t rowPins[ROWS] = {8, 9, 10, 11}; 
uint8_t colPins[COLS] = {12, 13, 14, 15}; 

int cpt = 0;
char code [4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
	while (!Serial) {};
  delay(100);
  scanner.Init();
  scanner.Scan();
  delay(1000);

  mcp.begin(0x20); 
  
  for(uint8_t i = 0;i<=7;i++){
    mcp.pinMode(i,OUTPUT);
  }

  for(uint8_t i = 0;i<ROWS;i++){
  mcp.pinMode(rowPins[i],OUTPUT);
  }
  for(uint8_t i = 0;i<COLS;i++){
    mcp.pinMode(colPins[i],INPUT);
  }

  mcp.digitalWrite(0,LOW);
  mcp.digitalWrite(1,HIGH);
  mcp.digitalWrite(2,HIGH);
  mcp.digitalWrite(3,LOW);

  mcp.digitalWrite(4,HIGH);
  mcp.digitalWrite(5,HIGH);
  mcp.digitalWrite(6,LOW);
  mcp.digitalWrite(7,HIGH);

}

void loop() {
  // put your main code here, to run repeatedly

  for(uint8_t i = 0;i<ROWS;i++){
    mcp.digitalWrite(rowPins[i],HIGH);
    for(uint8_t j = 0;j<COLS;j++){
      if(mcp.digitalRead(colPins[j])){
       Serial.println(hexaKeys[i][j]);
        code[cpt] = hexaKeys[i][j];
        cpt++;
        if(cpt==4){
          cpt=0;
          //Serial.print(code);
          for(int k=0;k<4;k++){
            Serial.print(code[k]);
          }
          Serial.print('\n');
          if(code[0]=='4' && code[1] =='9' && code[2] == '6' && code[3] == '5'){
            Serial.println("code ok");
          }
          else{
            Serial.println("code refusé");
          }
          //memset(code,0,sizeof(code));
        }  
      }
    }
    mcp.digitalWrite(rowPins[i],LOW);
  }
 delay(200);

}

