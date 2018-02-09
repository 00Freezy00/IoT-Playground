#include <Arduino.h>

int SensorPin = D6;
int val = 0;
int pirState = LOW;

void setup() {
    Serial.begin(115200);
    Serial.println();
    pinMode(SensorPin,INPUT);
    pinMode(BUILTIN_LED, OUTPUT);
}

void loop() {
  val = digitalRead(SensorPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(BUILTIN_LED, LOW);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(BUILTIN_LED, HIGH); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }
}
