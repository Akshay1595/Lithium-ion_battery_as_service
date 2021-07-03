#include "Arduino.h"
#include "../inc/my_led.h"
#include "../inc/config.h"

#define LOAD_LED_PIN                (39)        //RED_LED on board D10
#define CAN_DATA_LED_PIN            (41)        //WHITE_LED on board D11
#define UPDATE_GUI_TASK_LED_PIN     (43)        //GREEN_LED on board D12
#define CHARGING_INDICATOR_LED_PIN  (45)        //BLUE_LED on board D9
#define GET_TOGGLE_STATE_OF(X)      ((X)?LOW:HIGH)

typedef struct my_led {
    uint8_t pin;
    uint8_t curr_state;
}led;

// data for the file
led load_led, can_data_led, update_gui_task_led, charging_indicator;

// apis for the file
static inline void led_set(led* led_to_set) {
    digitalWrite(led_to_set->pin, HIGH);
    led_to_set->curr_state = HIGH;
}

static inline void led_clear(led* led_to_clear) {
    digitalWrite(led_to_clear->pin, LOW);
    led_to_clear->curr_state = LOW;
}


static inline void led_update_state(led* led_to_update, uint8_t state) {
    led_to_update->curr_state = state;
    digitalWrite(led_to_update->pin, state);
}

static inline bool led_get_current_state(led* led_to_get_status) {
    return led_to_get_status->curr_state;
}

static inline void led_toggle_state(led* led_to_toggle) {
    led_update_state(led_to_toggle, GET_TOGGLE_STATE_OF(led_to_toggle->curr_state));
}

static inline void led_set_can_data_led(void) {
    led_set(&can_data_led);
}

static inline void led_clear_can_data_led(void) {
    led_clear(&can_data_led);
}

void led_toggle_can_data_led(void) {
    led_toggle_state(&can_data_led);
}

void led_set_load_led(void) {
    led_set(&load_led);
}

void led_set_charging_indicator(void) {
    led_set(&charging_indicator);
}

void led_clear_charging_indicator(void) {
    led_clear(&charging_indicator);
}

void led_clear_load_led(void) {
    led_clear(&load_led);
}

static inline void led_toggle_load_led(void) {
    led_toggle_state(&load_led);
}

static inline void led_set_gui_task_led(void) {
    led_set(&update_gui_task_led);
}

inline void led_clear_gui_task_led(void) {
    led_clear(&update_gui_task_led);
}

void led_toggle_gui_task_led(void) {
    led_toggle_state(&update_gui_task_led);
}

void led_setup(void) {

    load_led.pin = LOAD_LED_PIN; load_led.curr_state = LOW;
    can_data_led.pin = CAN_DATA_LED_PIN; can_data_led.curr_state = LOW;
    update_gui_task_led.pin = UPDATE_GUI_TASK_LED_PIN; update_gui_task_led.curr_state = LOW;
    charging_indicator.pin = CHARGING_INDICATOR_LED_PIN;

    pinMode(load_led.pin, OUTPUT);
    pinMode(can_data_led.pin, OUTPUT);
    pinMode(update_gui_task_led.pin, OUTPUT);
    pinMode(charging_indicator.pin, OUTPUT);

    // Flash all LEDs to indicate initial start
    led_set_load_led();
    led_set_can_data_led();
    led_set_gui_task_led();
    led_set_charging_indicator();
    delay(1500);

    led_clear_load_led();
    led_clear_gui_task_led();
    led_clear_gui_task_led();
    led_clear_charging_indicator();
    delay(1500);

    debug_log("Led Setup Complete!");
}