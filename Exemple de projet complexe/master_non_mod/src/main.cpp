#include <Arduino.h>

#include <WiFi.h>
//#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <WiFiClient.h>
#include <PubSubClient.h> 
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MCP23017.h>


#define WIFI_SSID "Nilort 2.0"
#define WIFI_PASS "0483408720"
//#define WIFI_SSID "juanito pepito"
//#define WIFI_PASS "aaaa1230"


//
#define BP1 0     //raz mdp
#define BP2 36
// client
WiFiClient espClient;               //NODERED
PubSubClient client(espClient);     //NODERED   
//
Adafruit_MCP23017 mcp;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
//
const char* mqtt_server ="192.168.1.23";   //NODERED

LiquidCrystal_I2C lcd(0x27,20,4);

bool mdp = 0;
int cpt = 0;
int raw;
int col;
char code [4];
char nb[16] = {0x31,0x32,0X33,0x41,0x34,0x35,0x36,0x42,0x37,0x38,0x39,0x43,0x2A,0x30,0x23};
char chiffre = 0;

bool val_code(){ 
  if (mcp.digitalRead(0)){
    raw = 1;
    cpt+=1;
    if (mcp.digitalRead(4)){
      col = 1;
      chiffre = nb[0];
    }
    if (mcp.digitalRead(5)){
      col = 2;
      chiffre = nb[1];
    }
    if (mcp.digitalRead(6)){
      col = 3;
      chiffre = nb[2];
    }
    if (mcp.digitalRead(7)){
      col = 4;
      chiffre = nb[3];
    }
    code[(cpt-1)] = chiffre;
    Serial.print(raw);
    Serial.print(";");
    Serial.println(col);
  }
  if (mcp.digitalRead(1)){
    raw = 2;
    cpt+=1;
    if (mcp.digitalRead(4)){
      col = 1;
      chiffre = nb[4];
    }
    if (mcp.digitalRead(5)){
      col = 2;
      chiffre = nb[5];
    }
    if (mcp.digitalRead(6)){
      col = 3;
      chiffre = nb[6];
    }
    if (mcp.digitalRead(7)){
      col = 4;
      chiffre = nb[7];
    }
    code[(cpt-1)] = chiffre;
    Serial.print(raw);
    Serial.print(";");
    Serial.println(col);
  }
  if (mcp.digitalRead(2)){
    raw = 3;
    cpt+=1;
    if (mcp.digitalRead(4)){
      col = 1;
      chiffre = nb[8];
    }
    if (mcp.digitalRead(5)){
      col = 2;
      chiffre = nb[9];
    }
    if (mcp.digitalRead(6)){
      col = 3;
      chiffre = nb[10];
    }
    if (mcp.digitalRead(7)){
      col = 4;
      chiffre = nb[11];
    }
    code[(cpt-1)] = chiffre;
    Serial.print(raw);
    Serial.print(";");
    Serial.println(col);
  }
  if (mcp.digitalRead(3)){
    raw = 4;
    cpt+=1;
    if (mcp.digitalRead(4)){
      col = 1;
      chiffre = nb[12];
    }
    if (mcp.digitalRead(5)){
      col = 2;
      chiffre = nb[13];
    }
    if (mcp.digitalRead(6)){
      col = 3;
      chiffre = nb[14];
    }
    if (mcp.digitalRead(7)){
      col = 4;
      chiffre = nb[15];
    }
    code[(cpt-1)] = chiffre;
    Serial.print(raw);
    Serial.print(";");
    Serial.println(col);
  }

  if (cpt == 4){
    cpt=0;
    Serial.print("Code : ");
    Serial.println(code);
    if(code[0]== 0x00){
        if(code[1]==0x01){
            if(code[2]==0x02){
                if(code[3]==0x03){
                    Serial.print("code validé");
                    //return true;
                    return true;
                }
                else{
                    Serial.print("code refusé");
                }
            }
            else{
                Serial.print("code refusé");
            }
        }
        else{
            Serial.print("code refusé");
        }
    }
    else{
        Serial.print("code refusé");
    }
    memset(code,0,sizeof(code));
  }
return false;
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
      // ... and resubscribe
      client.subscribe("LIGHT1");
      client.subscribe("LIGHT2");
      client.subscribe("VANNE");
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

void setup() {
  // put your setup code here, to run once:
  //ini pins
  pinMode(BP1,INPUT);
  pinMode(BP2,INPUT);
  //moniteur dérie
  Serial.begin(9600);
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server,1883);    //NODERED
  client.setCallback(callback);         //NODERED
  //----------------------------------------------------LCD
  lcd.init(); 
  lcd.backlight();

  //----------------------------------------------------MCP
  mcp.begin();      // use default address 0

  mcp.pinMode(0, INPUT);
  mcp.pinMode(1, INPUT);
  mcp.pinMode(2, INPUT);
  mcp.pinMode(3, INPUT);
  mcp.pinMode(4, INPUT);
  mcp.pinMode(5, INPUT);
  mcp.pinMode(6, INPUT);
  mcp.pinMode(6, INPUT);

  mcp.pinMode(21, OUTPUT);
  mcp.pinMode(22, OUTPUT);
  mcp.pinMode(23, OUTPUT);
  mcp.pinMode(24, OUTPUT);
  mcp.pinMode(25, OUTPUT);
  mcp.pinMode(26, OUTPUT);
  mcp.pinMode(27, OUTPUT);
  mcp.pinMode(28, OUTPUT);

  //----------------------------------------------------SERVER


  // Start server
  server.begin();

  // Init coms
  Serial.print("saisir mdp");
  lcd.print("saisir mdp");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(mdp){

  }
  else{
    if(digitalRead(BP1)){
      //memset(code,'0',sizeof(code));
    }
    mdp = val_code();
  }
}