#include "Arduino.h"
#include "my_can.h"

static uint8_t SOC = 100;
static uint32_t discharge_rate = 1123;
static uint32_t charge_rate = 250;
static time_to_full_charge time_remaining = {2, 30};

void can_update_data(void) {
}

uint8_t can_get_soc(void) {
    return SOC;
}

uint32_t can_get_discharge_rate(void) {
    return discharge_rate;
}

uint32_t can_get_charge_rate(void) {
    return charge_rate;
}

time_to_full_charge can_get_time_to_charge(void) {
    return time_remaining;
}

bool can_is_charging(void) {
    return false;
}