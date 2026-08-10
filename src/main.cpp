#include <Arduino.h>

// --- Конфігурація (піни, константи) ---
constexpr uint8_t RELAY_PIN_IN = 4;
constexpr uint8_t RELAY_PIN_OUT = 5;
constexpr uint16_t INTERVAL = 500;
constexpr uint8_t MEASUREMENTS_COUNT = 10;

// --- Стан керування реле (toggle-таймер) ---
uint32_t lastToggle = 0;
bool relayCommand = false;
bool allMeasurementsDone = false;

// --- Стан вимірювання ---
uint32_t commandSentTime = 0;
bool waitingForResponse = false;
uint8_t measurementIndex = 0;
uint32_t sentTimeList[MEASUREMENTS_COUNT] = {};

// --- Дані, спільні з ISR (обов'язково volatile) ---
volatile bool signalReceived = false;
volatile uint32_t signalTime = 0;

void IRAM_ATTR relayPinOn() {
  signalReceived = true;
  signalTime = micros();
}

void printResults() {
	uint32_t sum = 0;
	Serial.println("\n ==== Measurement start ====");
	for (uint8_t i = 0; i < MEASUREMENTS_COUNT; i++) {
		Serial.printf("Measurement %d: %lu us\n", i + 1, sentTimeList[i]);
		sum += sentTimeList[i];
	}
	Serial.printf("Average: %lu us\n", sum / MEASUREMENTS_COUNT);
	Serial.println("==== Measurement finish ====");
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

	if (!allMeasurementsDone) {

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
				uint32_t delayInOut = signalTime - commandSentTime;

				sentTimeList[measurementIndex] = delayInOut;
				measurementIndex++;

				Serial.printf("Relay response time: %lu us\n", delayInOut);

				if (measurementIndex >= MEASUREMENTS_COUNT) {
					allMeasurementsDone = true;
				}
			}
		}

	} else {
		static bool printed = false;
		if (!printed) {
			printResults();
			printed = true;
		}
	}
}