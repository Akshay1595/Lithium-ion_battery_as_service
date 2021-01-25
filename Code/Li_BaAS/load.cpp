#include <Arduino.h>
#include "my_load.h"

#define LOAD_PIN        (13)

static bool load_connected = false;

void load_setup(void) {
    pinMode(LOAD_PIN,OUTPUT);
    digitalWrite(LOAD_PIN,LOW);
    load_connected = false;
}

void load_enable_discharge(void) {
    digitalWrite(LOAD_PIN,HIGH);
    load_connected = true;
}

void load_disable_discharge(void) {
    digitalWrite(LOAD_PIN,LOW);
    load_connected = false;
}

bool load_is_connected(void) {
    return load_connected;
}