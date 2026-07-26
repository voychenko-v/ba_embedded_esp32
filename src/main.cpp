#include <Arduino.h>

#define LED_BLUE_PIN 41
#define LED_RED_PIN 40
#define BUTTON_OUT_PIN 42
#define BUTTON_IN_PIN 0

const int delayMax = 500;
const int delayMin = 100;
const int delayStep = 100;
int delayValue = 100;

void blink(int pin, int duration) {
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
}

int stateButton(int pin){
  int stateBotton = digitalRead(pin);
  delay(50);
  return stateBotton;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(BUTTON_OUT_PIN, INPUT_PULLDOWN); 
  pinMode(BUTTON_IN_PIN, INPUT_PULLUP);
  Serial.println("GPIO система ініціалізована");
}

void loop() {

  int stateButtonOut = stateButton(BUTTON_OUT_PIN);
  int stateButtonIn = stateButton(BUTTON_IN_PIN);

  if (stateButtonOut == HIGH && stateButtonIn == LOW) {
    delayValue = 1000;
  } else if (stateButtonOut == HIGH) {
    delayValue = (delayValue <= delayMin) ? delayMin : delayValue - delayStep;
  } else if (stateButtonIn == LOW) {
    delayValue = (delayValue >= delayMax) ? delayMax : delayValue + delayStep;
  }

  Serial.printf("Затримка мигання: %d\n", delayValue);
  
  blink(LED_BLUE_PIN, delayValue);
  blink(LED_RED_PIN, delayValue);
  
}