#include <Arduino.h>

#define PIN_BUILDIN 13
#define BLINK_DELAY 2000

#include <stdarg.h>

void log(const char *fmt, ...) {
  char buf[64];

  va_list args;
  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  va_end(args);

  Serial.println(buf);
}

void setup() {
  Serial.begin(115200);

  Serial.println("Hello World!");
  pinMode(PIN_BUILDIN, OUTPUT);

  log("Test %s %d", "test", 10);
}

void loop() {
  digitalWrite(PIN_BUILDIN, HIGH);
  delay(BLINK_DELAY);
  digitalWrite(PIN_BUILDIN, LOW);
  delay(BLINK_DELAY);
}