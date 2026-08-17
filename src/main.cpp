#include <Arduino.h>

#define BUTTON_PIN         2
#define POLL_INTERVAL_MS   10
#define DEBOUNCE_TIME_MS   50

uint16_t pressed_count = 0;
uint16_t released_count = 0;

typedef enum {
    BTN_IDLE = 0,
    BTN_PRESS_DEBOUNCE,
    BTN_PRESSED,
    BTN_RELEASE_DEBOUNCE
} button_state_t;

typedef struct {
    button_state_t state;
    unsigned long  state_enter_tick;
    unsigned long  last_poll_tick;
    bool           event_pressed;
    bool           event_released;
} button_fsm_t;

static button_fsm_t btn = {
    .state = BTN_IDLE,
    .state_enter_tick = 0,
    .last_poll_tick = 0,
    .event_pressed = false,
    .event_released = false
};

void button_fsm_update(void);
bool button_is_pressed(void);
bool button_was_just_pressed(void);
bool button_was_just_released(void);

void button_fsm_update(void)
{
    uint32_t now = millis();

    if ((now - btn.last_poll_tick) < POLL_INTERVAL_MS) {
        return;
    }
    btn.last_poll_tick = now;

    uint8_t pin_state = digitalRead(BUTTON_PIN);

    switch (btn.state) {

    case BTN_IDLE:
        if (pin_state == HIGH) {
            btn.state = BTN_PRESS_DEBOUNCE;
            btn.state_enter_tick = now;
        }
        break;

    case BTN_PRESS_DEBOUNCE:
        if (pin_state == LOW) {
            btn.state = BTN_IDLE;
        } else if ((now - btn.state_enter_tick) >= DEBOUNCE_TIME_MS) {
            btn.state = BTN_PRESSED;
            btn.event_pressed = true;
        }
        break;

    case BTN_PRESSED:
        if (pin_state == LOW) {
            btn.state = BTN_RELEASE_DEBOUNCE;
            btn.state_enter_tick = now;
        }
        break;

    case BTN_RELEASE_DEBOUNCE:
        if (pin_state == HIGH) {
            btn.state = BTN_PRESSED;
        } else if ((now - btn.state_enter_tick) >= DEBOUNCE_TIME_MS) {
            btn.state = BTN_IDLE;
            btn.event_released = true;
        }
        break;
    }
}

bool button_is_pressed(void) { return btn.state == BTN_PRESSED; }

bool button_was_just_pressed(void)
{
    if (btn.event_pressed) {
        btn.event_pressed = false;
        return true;
    }
    return false;
}

bool button_was_just_released(void)
{
    if (btn.event_released) {
        btn.event_released = false;
        return true;
    }
    return false;
}

void setup()
{
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT);
}

void loop()
{
    button_fsm_update();

    if (button_was_just_pressed()) {
        pressed_count++;
        Serial.printf("Button pressed: %d\n", pressed_count);
    }
    if (button_was_just_released()) {
        released_count++;
        Serial.printf("Button released: %d\n", released_count);
    }
}