#include <Arduino.h>
#include <ST_HW_HC_SR04.h>
#include <ESP8266WiFi.h>
#include "ESPAsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include <Hash.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"
#include <WiFiClient.h>

const int ECHO = D6;
const int TRI = D7;
ST_HW_HC_SR04 ultrasonicSensor(TRI, ECHO,23200);

AsyncWebServer server(8080);
AsyncWebSocket ws("/ws"); // access at ws://[esp ip]/ws
AsyncEventSource events("/events"); // event source (Server-Sent events)

const char* ssid = "RelaySensor";
const char* password = "AllHands2017";

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
    Serial.begin(115200);
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
        int hitTime = ultrasonicSensor.getHitTime(); // In microseconds
        float distance = hitTime / 58.5 *2;
        String message = "The hit time was " + String(hitTime) + " microseconds\n" + "The distance is" + String(distance) + " cm.\n";
        AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", message);
        response->addHeader("Server","ESP Async Web Server");
        request->send(response);
    }
    );

    // Catch-All Handlers
    // Any request that can not find a Handler that canHandle it
    // ends in the callbacks below.
    server.onNotFound(onRequest);
    server.onFileUpload(onUpload);
    server.onRequestBody(onBody);

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
  int hitTime = ultrasonicSensor.getHitTime(); // In microseconds
  int distance = hitTime / 58.5 * 2;
  String message = "Distance: " + String(distance) + " cm\n";
  Serial.print(message);
  delay(1500);
}
