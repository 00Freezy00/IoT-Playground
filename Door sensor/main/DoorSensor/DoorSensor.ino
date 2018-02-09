#include <Arduino.h>
int pin = D7;
volatile int state = false;
volatile int flag = false;
const char* door_state = "closed";

unsigned long previousMillis = 0;
const long interval = 50;

void changeDoorStatus() {

    unsigned long currentMillis = millis();

    if(currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        state = !state;
        if(state) {
            door_state = "opened";
        }
        else{
            door_state = "closed";
        }
        flag = true;
        Serial.println(state);
        Serial.println(door_state);
    }

}


void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("Preparing the Door Status Monitor project...");
  pinMode(pin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pin), changeDoorStatus, CHANGE);


}

void loop() {
}
