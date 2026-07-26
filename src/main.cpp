#include <Arduino.h>

#define LED_BLUE 41
#define LED_RED 40
#define LED_GREEN 42


const int cycle_count = 3;
const int delay_max = 300;
const int delay_min = 100;
const int delay_step = 100;

int delay_value = delay_max;
int mode = 0;

enum mode_type {
  TWO_LED_ONE_BLINK,
  TWO_LED_TWO_BLINK,
  THREE_LED_ONE_BLINK
};

void blink(int pin, int duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
}

void setup() {
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}

void loop() {
  
  switch (mode)
  {
  case TWO_LED_ONE_BLINK:
    for(int i=0; i < cycle_count; ++i){
      blink(LED_BLUE, delay_value);
      blink(LED_RED, delay_value);
    }
    break;

  case TWO_LED_TWO_BLINK:
    for(int i=0; i < cycle_count; ++i){
      blink(LED_BLUE, delay_value);
      delay(delay_value);
      blink(LED_BLUE, delay_value);

      blink(LED_RED, delay_value);
      delay(delay_value);
      blink(LED_RED, delay_value);
    }
    break;
  
  case THREE_LED_ONE_BLINK:
    for(int i=0; i < cycle_count; ++i){
      blink(LED_BLUE, delay_value);
      blink(LED_GREEN, delay_value);
      blink(LED_RED, delay_value);
    }
  break;
  }
  
  mode = (mode + 1) % 3;

  if (mode == 0) {
    delay_value = (delay_value <= delay_min) ? delay_max : delay_value - delay_step;
}
  
}
