//salve don't connect to wifi  they  just  transmit 
#include <Arduino.h>
#include "nrf_slave.h"
#include <DHT.h>
#include <max6675.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define thermoDO 19 
#define thermoCS 27
#define thermoCLK 18

#define DHTPIN 34
#define DHTTYPE DHT11   // DHT 11

#define RGB_R 16
#define RGB_G 13
#define RGB_B 17

#define ldr 39

#define pot 33
#define relais 25
#define bp1 0
#define bp2 36
#define tact 14

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

LiquidCrystal_I2C lcd(0x27,20,4); //vérifier lib pour arg

DHT dht(DHTPIN, DHTTYPE);

int dht_last = 0;
float h,t,tmc; // var dht tmc
int l; // var lumi 

bool manu,state;

float seuil = 18;

void setup() {
  // put your setup code here, to run once:
  pinMode(bp1,INPUT);
  pinMode(bp2,INPUT);
  pinMode(relais,OUTPUT);

  Serial.begin(9600);

  // RGB
  #if defined(ARDUINO_ARCH_ESP32) // ESP32 pinMode
    // assign rgb pins to channels
    ledcAttachPin(RGB_R,1);
    ledcAttachPin(RGB_G,2);
    ledcAttachPin(RGB_B,3);

    // init. channels
    ledcSetup(1, 1000, 8);
    ledcSetup(2, 1000, 8);
    ledcSetup(3, 1000, 8);
  #else
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
  #endif

  // NRF
  setup_nrf();

  // lcd
  lcd.init();                      // initialize the lcd 
  lcd.backlight();

  //dht
  dht.begin();

  //inital lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Slave board");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(bp1)){
    lcd.clear();
    lcd.setCursor(0,3);
    lcd.print("PROG MODE");
    delay(10);
    while(!digitalRead(bp1)){
      seuil = analogRead(pot);
      lcd.setCursor(1,3);
      lcd.print("Seuil lumi :");
      lcd.print(seuil);
    }
  }
  else{
    if(dht_last<= millis()){
      l = analogRead(ldr);
      h = dht.readHumidity();
      t = dht.readTemperature();
      tmc = thermocouple.readCelsius();
      Serial.println("mesures dht");
      Serial.println(h);
      Serial.println(t);
      Serial.println("mesures tmc");
      Serial.println(tmc);
      lcd.clear();
      lcd.setCursor(0,5);
      lcd.print("Values");
      lcd.setCursor(1,1);
      lcd.print("Temp : ");
      lcd.print(tmc);
      lcd.print("°C");
      lcd.setCursor(1,11);
      lcd.print("humi ");
      lcd.print(h);
      radio_send_data(0x54 ,tmc);
      delay(10);
      radio_send_data(0x48 ,h);
      delay(10);
      radio_send_data(0x4C ,l);
      if(manu){
        if(l<=1000){                            // utiliser manu pour introduire activation manu auto
          digitalWrite(relais,HIGH);            // pour l'int 100% auto
        }
        if(l>=3000){
          digitalWrite(relais,LOW);
        }
      }
      if(tmc > 24){
        ledcWrite(1,255);
        ledcWrite(2,0);
        ledcWrite(3,0);
      }
      if(tmc >= 18){
        ledcWrite(1,0);
        ledcWrite(2,255);
        ledcWrite(3,0);
      }
      if(tmc < 18){
        ledcWrite(1,0);
        ledcWrite(2,0);
        ledcWrite(3,255);
      }
      dht_last = millis() + 60000;
    }
    if(digitalRead(bp2)){
      if(state){
        digitalWrite(relais,LOW);
        state = false;
      }
      if(!state){
        digitalWrite(relais,HIGH);
        state = true;
      }
    }
    if(touchRead(tact)<20){
      if(manu){
        manu = false;
      }
      if(!manu){
        manu = true;
      }
    }
  }
} 