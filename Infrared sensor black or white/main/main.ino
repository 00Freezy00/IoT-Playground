#include <dummy.h>
#include "tcrt5k.h"
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include <Hash.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

TCRT5000 tcrt(D7);

AsyncWebServer server(8080);
AsyncWebSocket ws("/ws"); // access at ws://[esp ip]/ws
AsyncEventSource events("/events"); // event source (Server-Sent events)

const char *ssid = "RelaySensor";
const char *password = "AllHands2017";

//flag to use from web update to reboot the ESP
bool shouldReboot = false;

bool sensorResult = false;

void onRequest(AsyncWebServerRequest *request){
  //Handle Unknown Request
  request->send(404);
}

void onBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  //Handle body
}

void onUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
  //Handle upload
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  //Handle WebSocket event
}

void setup() {
    Serial.begin(115200);  // We'll see the output from Serial
    Serial.println();
    Serial.print("Configuring access point...");
    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    // attach AsyncWebSocket
    ws.onEvent(onEvent);
    server.addHandler(&ws);

    // attach AsyncEventSource
    server.addHandler(&events);

    // respond to GET requests on URL /
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      AsyncResponseStream *response = request->beginResponseStream("text/json");
      DynamicJsonBuffer jsonBuffer;
      JsonObject &root = jsonBuffer.createObject();
      root["isClose"] = sensorResult;
      root.printTo(*response);
      request->send(response);
    });


    // Catch-All Handlers
    // Any request that can not find a Handler that canHandle it
    // ends in the callbacks below.
    server.onNotFound(onRequest);
    server.onFileUpload(onUpload);
    server.onRequestBody(onBody);

    server.begin();
    Serial.println("HTTP server started");
}



void loop(){
    if (tcrt.isClose()) {  // check tcrt5k
        sensorResult = true;
    } else {
        sensorResult = false;
    }
    delay(333);
}
