#ifndef CHARGER_H
#define CHARGER_H

#include "bms.h"

// charger global data
typedef struct time_t {
    uint8_t hrs;
    uint8_t mins;
}time_to_full_charge;

typedef struct charge_current_t  {
    float input_vltg;
    float input_current;    
    time_to_full_charge full_chrge_time;
}charger_data;

//---------------------------------------------------------------------------------//
/* Charger Information Message */
#define CHARGER_MSG_ID              (0x18FF50E5)

// function
extern charger_data charger_update_data(bms_data bms_values);
extern time_to_full_charge charger_get_time_to_full_charge(void);
extern batt_i charger_get_charging_current(void);
extern batt_v charger_get_charging_voltage(void);

#endif // CHARGER_H