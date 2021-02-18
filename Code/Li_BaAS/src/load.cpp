#include <Arduino.h>
#include "../inc/my_load.h"
#include "../inc/my_led.h"
#include "../inc/config.h"

#define LOAD_PIN        (30)

static bool load_connected = false;

void load_setup(void) {
    pinMode(LOAD_PIN,OUTPUT);
    digitalWrite(LOAD_PIN,LOW);
    load_connected = false;
    led_clear_load_led();
    debug_log("Load Setup done!");
}

void load_enable_discharge(void) {
    digitalWrite(LOAD_PIN,HIGH);
    load_connected = true;
    led_set_load_led();
}

void load_disable_discharge(void) {
    digitalWrite(LOAD_PIN,LOW);
    load_connected = false;
    led_clear_load_led();
}

bool load_is_connected(void) {
    return load_connected;
}