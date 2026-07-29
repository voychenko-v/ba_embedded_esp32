#include <Arduino.h>

#define BUTTON_PIN 42

int16_t counter = 0;


void IRAM_ATTR reaction() {
  counter++;
  Serial.printf("Button Pressed! Count: %d\n", counter);
}


void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLDOWN);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), reaction, FALLING);
}

void loop() {

}