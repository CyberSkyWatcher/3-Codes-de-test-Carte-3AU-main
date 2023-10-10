#include "config.h"

/************************************************/
//Strip object
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

/************************************************/
// client
WiFiClient espClient;             
PubSubClient client(espClient);     
 
/************************************************/
//LCD
//LiquidCrystal_I2C lcd(0x27,20,4);
//OLED

/************************************************/
// Var programmes
bool mdp = 0;

/************************************************/
// Proto
void callback(char* topic, byte* payload, unsigned int length);
void reconnect(void);

void setup() {
  //moniteur dérie
  Serial.begin(9600);

  // Start I2C
  Wire.begin(); 

  //ini pins
  pinMode(BP1,INPUT);
  pinMode(BP2,INPUT);

  // RGB
  //rgb_setup(RGB_R,RGB_G,RGB_B);
  strip.begin();                //strip -- ruban = initialisation de /des leds
  strip.setBrightness(50);      //Fixe la luminosité de la led (0-255)

  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  //----------------------------------------------------LCD
  /*
  lcd.init(); 
  lcd.backlight();
  */

  //----------------------------------------------------SERVER
  setup_spiff();
  setup_server();

  //----------------------------------------------------MCP
  setup_mcp();

  //----------------------------------------------------nrf
  setup_nrf();

  //----------------------------------------------------MQTT-nr
  client.setServer(mqtt_server,1883);    //NODERED
  client.setCallback(callback);          //NODERED


  // Init coms
  Serial.println("saisir mdp");
  lcd.print("saisir mdp");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(mdp){
    if (!client.connected()) {   //NODERED
      //rgb_write(255,0,0);
      strip.setLedColorData(0, 255, 0, 0);
      strip.show();
      reconnect();

    }
    client.loop();                 //NODERED

  }
  else{
    if(digitalRead(BP1)){
    //memset(code,'0',sizeof(code));
    }
    mdp = val_code();
  }
}


void callback(char* topic, byte* payload, unsigned int length) {     //NODERED que faire quand un msg arrive
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char msg[32];
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg[i]=payload[i];
  }
  Serial.println();
  Serial.print(msg);
  Serial.println();
  memset(msg,0,sizeof(msg));
  if ((char)payload[0] == '0') {
  
  } 
  else
  {
    Serial.print("Unknown Payload");
  }
}

void reconnect(void) {     //NODERED mqqt connexion 
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      rgb_write(0,255,0);
      // ... and resubscribe
      client.subscribe("temp");
      client.subscribe("humi");
      client.subscribe("lumi");
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}