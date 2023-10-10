
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Wire.h>
#include "f_mcp.h"
#include "web_server.h"
#include "nrf_com.h"
#include "rgb.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"  
//#include <LiquidCrystal_I2C.h>

/***************************/

#define BP1 0     //raz mdp
#define BP2 36

#define B_tact 14

/***************************
 // PINs Carte 1.0 RGB 3Pins
#define RGB_R 16
#define RGB_G 13
#define RGB_B 17
*****************************/
//RGB config WS2812
#define LEDS_COUNT   1			//nombre de leds
#define LEDS_PIN     0			
#define CHANNEL   	 0			// comme évoqué avec la led 3pins --> pas de analogW sur esp32


/******************************************/
//Web

#define WIFI_SSID "SSID_WIFI"
#define WIFI_PASS "PASSWORD"
const char* mqtt_server ="192.168.1.255";  


#endif