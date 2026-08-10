#include <Arduino.h>

constexpr uint8_t RELAY_PIN_IN = 4;
constexpr uint8_t RELAY_PIN_OUT = 5;
constexpr uint16_t INTERVAL = 1000;

uint32_t lastToggle = 0;
uint32_t commandSentTime = 0;
bool relayCommand = false;
bool waitingForResponse = false;

volatile bool signalReceived = false;
volatile uint32_t signalTime = 0;

void IRAM_ATTR relayPinOn() {
  signalReceived = true;
  signalTime = micros();
}

void setup() {
	Serial.begin(115200);
	pinMode(RELAY_PIN_IN, OUTPUT);
	pinMode(RELAY_PIN_OUT, INPUT_PULLUP);
	digitalWrite(RELAY_PIN_IN, LOW);
	delay(1000);
	Serial.println("Relay test started");

	attachInterrupt(digitalPinToInterrupt(RELAY_PIN_OUT), relayPinOn, RISING);
}

void loop() {
	uint32_t now = millis();

	if (now - lastToggle >= INTERVAL) {
		lastToggle = now;
		relayCommand = !relayCommand;
		digitalWrite(RELAY_PIN_IN, relayCommand ? HIGH : LOW);

		if (relayCommand) {
			commandSentTime = micros();
			waitingForResponse = true;
		} else {
			waitingForResponse = false;
		}

		Serial.println(relayCommand ? "Relay ON" : "Relay OFF");
	}

	if (signalReceived) {
		signalReceived = false;

		if (waitingForResponse) {
			waitingForResponse = false;
			uint32_t delay_in_out = signalTime - commandSentTime;
			Serial.printf("Relay response time: %lu ms\n", delay_in_out);
		}
		
	}
}