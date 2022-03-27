#include <Arduino.h>

#define PIN_BUILDIN 13
#define BLINK_DELAY 2000

void setup() {
  Serial.begin(115200);

  Serial.println("Hello World!");
  pinMode(PIN_BUILDIN, OUTPUT);
}

void loop() {
  digitalWrite(PIN_BUILDIN, HIGH);
  delay(BLINK_DELAY);
  digitalWrite(PIN_BUILDIN, LOW);
  delay(BLINK_DELAY);
}