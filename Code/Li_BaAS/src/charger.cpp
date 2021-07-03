#include "math.h"
#include "Arduino.h"
#include "../inc/charger.h"
#include "../inc/config.h"
#include "../inc/bms.h"
#include "../inc/my_gui.h"

static charger_data g_charging_stats;

time_to_full_charge charger_get_time_to_full_charge(void) {
    return g_charging_stats.full_chrge_time;
}

batt_i charger_get_charging_current(void) {
    return g_charging_stats.input_current;
}

batt_v charger_get_charging_voltage(void) {
    return g_charging_stats.input_vltg;
}

charge_rate charger_get_charge_rate(void) {
    charge_rate curr_rate;
    curr_rate = charger_get_charging_voltage() * (abs(charger_get_charging_current()));
    return curr_rate;
}

charger_data charger_update_data(bms_data bms_values) {
    
    charger_data charger_stats;
    time_to_full_charge expected_time;
    expected_time.hrs = 0;
    expected_time.mins = 0;

    // obtain the values from BMS
    batt_i charger_current = abs(bms_values.current);
    batt_v charger_voltage = bms_values.batt_vltg;
    state_of_charge current_soc = bms_values.soc;

    // if charging going on then only extract charger data   
    if (bms_values.is_charging) {

        //get the battery capacities
        float battery_cap = (float) BATTERY_CAP_IN_WHR;
        float battery_current_cap = (current_soc * (float)battery_cap) / 100;

        float need_to_charge = battery_cap - battery_current_cap;
        float time_needed;
        if ( (int(need_to_charge) != 0) && (int(charger_current) != 0) && (int(charger_voltage) != 0) )
            time_needed = (need_to_charge) / (charger_current * charger_voltage);
        ///*
        char strForBatCap[8], strForBatCurrCap[8], strForNeedToCharg[8],strForTimeNeeded[8];
        char finalString[64] = {};
        dtostrf(battery_cap, 5, 2, strForBatCap);;
        dtostrf(battery_current_cap, 5, 2, strForBatCurrCap);
        dtostrf(need_to_charge, 5, 2, strForNeedToCharg);
        sprintf(finalString, "Stats: BattCap = %s CurCap = %s TimeNeeded = %s", strForBatCap, strForBatCurrCap, strForTimeNeeded);
        debug_log(finalString);
        //*/

        expected_time.hrs = int(time_needed);
        expected_time.mins = int ((time_needed - expected_time.hrs) * 60);
    }
    g_charging_stats.full_chrge_time = charger_stats.full_chrge_time = expected_time;
    g_charging_stats.input_current = charger_stats.input_current = charger_current;
    g_charging_stats.input_vltg = charger_stats.input_vltg = charger_voltage;

    return charger_stats;
}