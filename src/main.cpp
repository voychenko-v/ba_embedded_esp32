#include <Arduino.h>
#include "esp_task_wdt.h"

#define RELAY_PIN 1
#define WDT_TIMEOUT_S 6

hw_timer_t* timer1 = NULL;
volatile uint8_t secCounter = 0;

const uint8_t PERIOD_S = 6;
const uint8_t ON_S = 2;
volatile bool relayState = false;
volatile bool stateChanged = false;

void IRAM_ATTR onTimer1(){

    secCounter++;
    if(secCounter >= PERIOD_S){
        secCounter = 0;
    }

    bool newState = secCounter < ON_S;
    digitalWrite(RELAY_PIN, secCounter < ON_S ? LOW : HIGH);

    if (newState != relayState) {
        relayState = newState;
        stateChanged = true;
    }
}


void setup() {
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, HIGH);

    // Налаштування таймера
    timer1 = timerBegin(0, 80, true);
    timerAttachInterrupt(timer1, &onTimer1, true);
    timerAlarmWrite(timer1, 1000000, true);
    timerAlarmEnable(timer1);

    // Налаштування вачдога
    esp_task_wdt_init(WDT_TIMEOUT_S, true);
    esp_task_wdt_add(NULL);
}

void loop() {
    esp_task_wdt_reset();

    if (stateChanged) {
        stateChanged = false;
        Serial.printf("Реле: %s\n", relayState ? "УВІМКНЕНО" : "ВИМКНЕНО");
    }
    delay(200);
}