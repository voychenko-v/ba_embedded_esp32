#include <Arduino.h>

#define LED_PIN_1 1
#define LED_PIN_2 2
#define LED_PIN_3 42

#define INTERVAL_LED_1 200
#define INTERVAL_LED_2 500
#define INTERVAL_LED_3 1000

#define FLASH_DURATION 100 // має бути в два рази менше ніж найменший INTERVAL_LED_X

struct BlinkingLed {
	uint8_t pin;
	uint16_t interval;
	uint16_t flashDuration;
	uint32_t lastFlashTime;
	bool isOn;
};

BlinkingLed leds[] = {
	{LED_PIN_1, INTERVAL_LED_1, FLASH_DURATION, 0, false},
	{LED_PIN_2, INTERVAL_LED_2, FLASH_DURATION, 0, false},
	{LED_PIN_3, INTERVAL_LED_3, FLASH_DURATION, 0, false}
};

const size_t NUM_LEDS = sizeof(leds) / sizeof(leds[0]);

void setup() {
	for (size_t i = 0; i < NUM_LEDS; ++i) {
		pinMode(leds[i].pin, OUTPUT);
	}
}


void loop() {
	uint32_t now = millis();

	for (size_t i = 0; i < NUM_LEDS; ++i) {

		BlinkingLed &led = leds[i];
		
		if (!led.isOn && now - led.lastFlashTime >= led.interval ) {
			led.lastFlashTime = now;
			digitalWrite(led.pin, HIGH);
			led.isOn = true;
		}

		if (led.isOn && now - led.lastFlashTime >= led.flashDuration){
			digitalWrite(led.pin, LOW);
			led.isOn = false;
		}
	}
}