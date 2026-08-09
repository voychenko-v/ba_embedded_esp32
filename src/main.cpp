#include <Arduino.h>

constexpr uint8_t LED_PIN = 48;
constexpr uint8_t BUTTON_PIN = 0;
constexpr uint16_t PHASE_TIME = 1000;
constexpr uint32_t ITERATION_PRINT = 1000;
constexpr uint8_t DEBOUNCE_TIME = 50; 

uint32_t measureStart = 0;
uint32_t iterationCount = 0;
uint32_t lastButtonTime = 0;

volatile bool buttonPressed = false;

enum class LedState {
	off,
	on
};

enum class LedMode {
	blinking,
	alwaysOn,
	alwaysOff
};

LedMode currentMode = LedMode::blinking;

class Led {
public:
	explicit Led(uint8_t pin) : pin_(pin) {}

	void init() {
		pinMode(pin_, OUTPUT);
		set(LedState::off);
	}

	void set(LedState state) {
		digitalWrite(pin_, state == LedState::on ? HIGH : LOW);
	}

private:
	uint8_t pin_;
};

Led led(LED_PIN);

void IRAM_ATTR onButtonPress() {
  buttonPressed = true;
}

// Функція для зміни режиму блимання
void nextMode() {
	switch (currentMode) {
		case LedMode::blinking:  currentMode = LedMode::alwaysOn;  break;
		case LedMode::alwaysOn:  currentMode = LedMode::alwaysOff; break;
		case LedMode::alwaysOff: currentMode = LedMode::blinking;  break;
	}
}

void setup() {
	led.init();
	Serial.begin(115200);
	measureStart = micros();

	pinMode(BUTTON_PIN, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonPress, FALLING);
}

void loop() {

	if (buttonPressed) {
		buttonPressed = false;

		uint32_t now = millis();
		if (now - lastButtonTime >= DEBOUNCE_TIME) {
			lastButtonTime = now;
			nextMode();
		}
	}

	switch (currentMode) {
		case LedMode::blinking: {
			uint32_t phase = millis() / PHASE_TIME;
			led.set((phase % 2 == 0) ? LedState::on : LedState::off);
			break;
		}
		case LedMode::alwaysOn:
			led.set(LedState::on);
			break;
		case LedMode::alwaysOff:
			led.set(LedState::off);
			break;
	}

	iterationCount++;

	if (iterationCount >= ITERATION_PRINT) {
		uint32_t elapsed = micros() - measureStart;

		Serial.printf("1000 iterations took: %d us \n", elapsed);
		iterationCount = 0;
		measureStart = micros();
	}
}