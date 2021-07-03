#include "Arduino.h"
#include <mcp2515.h>
#include "../inc/config.h"
#include "../inc/bms.h"
#include "../inc/my_can.h"
#include "../inc/config.h"
#include "../inc/charger.h"
#include "../inc/my_gui.h"

static bms_data g_bms_data = {};

static bool bms_check_if_fault(batt_failure_level level) {
    if (level == NO_FAULT)
        return false;
    else
        return true;
}

static batt_i bms_get_current(u_bms_charge_discharge_current curr) {    
    // invalid condition
    if (curr.current.i_high > 0x27)
        return INVALID_CURRENT_VALUE;
    float current = (float)0;
    curr.bms_current -= 5000;
    current  = ( (float) curr.bms_current / (float) 10);
    return current;
}

static batt_v bms_get_vltg(u_bms_batt_vltg vltg) {

    if (vltg.vltg_st.batt_vltg_high > 39)
        return INVALID_VOLTAGE_VALUE;
    return ((float) vltg.batt_vltg * 0.1);
}

state_of_charge bms_get_soc(void) {
    return g_bms_data.soc;
}

batt_v bms_get_batt_vltg(void) {
    return g_bms_data.batt_vltg;
}

batt_i bms_get_batt_current(void) {
    return g_bms_data.current;
}

discharge_rate bms_get_discharge_rate(void) {
    discharge_rate curr_rate = ( bms_get_batt_vltg() * bms_get_batt_current() );
    return curr_rate;
}

status_of_charging bms_is_charging(void) {
    return g_bms_data.is_charging;
}

void bms_update_charging_status(status_of_charging is_charging) {
    g_bms_data.is_charging = is_charging;
}

void bms_update_data_of_bms_info1(struct can_frame* info_msg_1) {
    char string[256] = {};
    char v_string[16] = {};
    char i_string[16] = {};

    bms_info_msg_1* bms_info_1_t = (bms_info_msg_1 *)&(info_msg_1->data[0]);

    // get charging status
    if (bms_info_1_t->byte1.is_charger_connected) {
        g_bms_data.is_charging = bms_info_1_t->byte1.is_charging_on;
    }
    g_bms_data.is_charging = bms_info_1_t->byte1.is_charging_on;

    // get soc
    if (bms_info_1_t->soc <= 100)
        g_bms_data.soc = bms_info_1_t->soc;

    // battery pack current Low
    float i_value = bms_get_current(bms_info_1_t->current);
    if (i_value != INVALID_CURRENT_VALUE)
        g_bms_data.current = i_value;

    // battery pack voltage    
    float f_value = bms_get_vltg(bms_info_1_t->batt_vltg);
    if (f_value != INVALID_VOLTAGE_VALUE)
        g_bms_data.batt_vltg = f_value;

    // battery is fault?
    g_bms_data.is_fault = bms_check_if_fault(bms_info_1_t->failure_level);

    dtostrf(g_bms_data.batt_vltg, 5, 2, v_string);
    dtostrf(g_bms_data.current, 5, 2, i_string);

    sprintf(string, "Data Info1 Updated: soc = %d current = %s vltg = %s is_charging = %d is_fault = %d\n",
        g_bms_data.soc, i_string, v_string, g_bms_data.is_charging, g_bms_data.is_fault);
    debug_log(string);

    if (g_bms_data.is_charging) {
        charger_data charging_stats = charger_update_data(g_bms_data);
        char strForCurr[8],strForVltg[8];
        dtostrf(charging_stats.input_current, 5, 2, strForCurr);
        dtostrf(charging_stats.input_vltg, 5, 2, strForVltg);
        sprintf(string, "Charger_stats Updated: I= %s V= %s Hrs= %d Mins= %d\n",
            strForCurr, strForVltg, 
            charging_stats.full_chrge_time.hrs, charging_stats.full_chrge_time.mins);
        debug_log(string);
    }
}

void bms_update_data_of_bms_info2(struct can_frame* bms_info_msg_2) {
    // no code here
    // no need to decrypt the message
}

void bms_decrypt_can_msg(struct can_frame* canMsg) {

    char string_c[64] = {};
    can_msg_id* id = (can_msg_id*) (&(canMsg->can_id));
    debug_log("Inside BMS Decrypt CAN msg");
    
    if(id->msg_id == BMS_INFO_MSG_1_MSG_ID){
        
        // fill CAN message info 1
        bms_update_data_of_bms_info1(canMsg);

    } else if (id->msg_id == BMS_INFO_MSG_2_MSG_ID){
        
        // fill CAN message info 2
        bms_update_data_of_bms_info2(canMsg);
    }
    else
        return;
}