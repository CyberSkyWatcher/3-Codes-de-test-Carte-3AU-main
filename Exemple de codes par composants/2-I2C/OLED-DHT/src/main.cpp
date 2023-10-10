/********************************************* OLED + DHT ********************************************************/

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

/* Bibliothèques DHT --> adafruit propose la gestion de nombreux autres capteurs*/
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
/* Les deux  bibliothèques indispensables pour exploiter l'écran OLED*/
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // Largeur du LCD OLED
#define SCREEN_HEIGHT 64 // Hauteur du LCD OLED
#define OLED_RESET     -1 // Pin de reset du LCD --> si partagée avec l'ESP fixer à "-1"
#define SCREEN_ADDRESS 0x3C // Adresse du LCD souvent 0x3C ou 0x3D

// Declaration  SSD1306 display  I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// dht type --> dht11
#define DHTPIN 26
#define DHTTYPE    DHT11     // DHT 11

DHT_Unified dht(DHTPIN, DHTTYPE); // Déclaration DHT11

/* Variables programme*/
int send_time;  // timer envoi de données
char dht_buffer [16]; 


void setup() {
  /* Initialisations*/
  Serial.begin(9600); 
  dht.begin();
  // SSD1306_SWITCHCAPVCC --> Génère le 3V3 en interne
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Boucle infinie si OLED n'a pas démarré
  }
  /* display()  est la fonction qui permet d'actualiser l'écran*/
  /* Si au  démarrage rien n'est communiqué --> affiche le slash adafruit par défaut*/
  display.display();

/***** Affichage des détails du  capteur *****/
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
/**********************************************************************************************/
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
/**********************************************************************************************/
  delay(2000); // Pause for 2 seconds
}

void loop() {
  if(send_time <= millis()){
/* Récupération des données du capteur*/
/*isnan --> indique simplement si la mesure n'a pas été possible*/
// Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    float t = event.temperature;
    if (isnan(event.temperature)) {
      Serial.println(F("Error reading temperature!"));
    }
    else {
      Serial.print(F("Temperature: "));
      Serial.print(event.temperature);
      Serial.println(F("°C"));
    }
// Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    float h = event.relative_humidity;
    if (isnan(event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    }
    else {
      Serial.print(F("Humidity: "));
      Serial.print(event.relative_humidity);
      Serial.println(F("%"));
    }
    delay(100);
 /*A présent on modifie l'affichage sur le LCD*/   
    display.clearDisplay();

    display.setTextSize(1); // Un char vaut 6*8 pixels
    display.setTextColor(SSD1306_WHITE);
    display.cp437(true);         // Use full 256 char 'Code Page 437' font
    display.setCursor(5, 0);
    display.println("Valeur Humidite :");
    display.setCursor(32,12);
    display.setTextSize(2); 
    display.println(h);

    display.drawLine(0,30,128,30,WHITE);

    display.setCursor(5,36);
    display.setTextSize(1); 
    display.println("Valeur Temperature :");
    display.setCursor(32,48);
    display.setTextSize(2); 
    display.println(t);

/* finalement on actualise*/
    display.display();      
/* La bibliothèque SSD_1306 possède de nombreuses fonctions dont voici un exemple*/
    //display.startscrollleft(0x00, 0x0F);
   
    send_time = millis() + 1000; // On fixe la prochaine lecture --> ici 1s
  }
}
