#include <Arduino.h>

#define ADC_PIN 10
#define ADC_BIT_RATE 12
#define ADC_STEPS ((1 << ADC_BIT_RATE) - 1)

void setup() {
	Serial.begin(115200);
	delay(1000);
    analogReadResolution(ADC_BIT_RATE);
    analogSetPinAttenuation(ADC_PIN, ADC_11db);
	Serial.println("|======|=============|============|===========|");
	Serial.println("| RAW  | U calc (mV) | U lib (mV) | Error (%) |");
	Serial.println("|======|=============|============|===========|");
}

void loop() {
	uint16_t raw_data = analogRead(ADC_PIN);
	uint16_t mv_data = analogReadMilliVolts(ADC_PIN);

	float u_calc = ((float)raw_data / ADC_STEPS) * 3300.0;
	float error = u_calc - mv_data;
	float error_rel = (error / mv_data) * 100;

	Serial.printf("| %4d | %11.2f | %10lu | %+9.2f |\n", raw_data, u_calc, mv_data, error_rel);
	Serial.println("|------|-------------|------------|-----------|");

	delay(100);
}