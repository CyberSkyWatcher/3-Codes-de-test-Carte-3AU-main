
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void setup_spiff(void){
    //----------------------------------------------------SPIFFS
    if(!SPIFFS.begin())
    {
        Serial.println("Erreur SPIFFS...");
        return;
    }

    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    while(file)
    {
        Serial.print("File: ");
        Serial.println(file.name());
        file.close();
        file = root.openNextFile();
    }

    Serial.println("Successfully Ended");
}

void setup_server(void){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    {
        // Route for root / web page
        request->send(SPIFFS, "/index.html","text/html");
        //request->send(200, "text/html",index_html);  --> ligne pour css mais je fais avec le lien pour l'instant
    });
    // Start server
    server.begin();
}