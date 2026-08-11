#include <Arduino.h>

constexpr uint8_t POT_READ_PIN = 1;
constexpr uint8_t MOTOR_PWM_PIN = 2;
constexpr uint32_t PWM_PERIOD = 100; // 100 мкс = 10 кГц
constexpr uint16_t ADC_MAX = 4095;
constexpr uint32_t READ_INTERVAL = 20;

uint32_t lastReadTime = 0;
uint32_t periodStart = 0;
uint32_t onTime = 0;

void setup() {
	Serial.begin(115200);
	pinMode(POT_READ_PIN, INPUT);
	pinMode(MOTOR_PWM_PIN, OUTPUT);

	periodStart = micros();
	lastReadTime = millis();
}

void loop() {
	uint32_t nowMs = millis();
	uint32_t nowUs = micros();

	if (nowMs - lastReadTime >= READ_INTERVAL) {
		lastReadTime = nowMs;
		uint16_t adcValue = analogRead(POT_READ_PIN);
		onTime = map(adcValue, 0, ADC_MAX, 0, PWM_PERIOD);
	}

	uint32_t elapsedInPeriod = nowUs - periodStart;
	if (elapsedInPeriod >= PWM_PERIOD) {
		periodStart = nowUs;
		elapsedInPeriod = 0;
	}

	digitalWrite(MOTOR_PWM_PIN, elapsedInPeriod < onTime ? HIGH : LOW);
}