#include <Arduino.h>

constexpr uint8_t RELAY_PIN_IN = 4;
constexpr uint16_t INTERVAL = 1000;

uint32_t lastToggle = 0;
bool relayState = false;

void setup() {
	Serial.begin(115200);
	pinMode(RELAY_PIN_IN, OUTPUT);
	digitalWrite(RELAY_PIN_IN, LOW);
	delay(1000);
	Serial.println("Relay test started");
}

void loop() {
	uint32_t now = millis();

	if (now - lastToggle >= INTERVAL) {
		lastToggle = now;
		relayState = !relayState;
		digitalWrite(RELAY_PIN_IN, relayState ? HIGH : LOW);

		Serial.println(relayState ? "Relay ON" : "Relay OFF");
	}
}