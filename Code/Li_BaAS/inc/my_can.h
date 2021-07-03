#ifndef MY_CAN_H
#define MY_CAN_H
#include "charger.h"
#include "bms.h"

// Function declarations
extern uint8_t can_get_soc(void);
extern discharge_rate can_get_discharge_rate(void);
extern charge_rate can_get_charge_rate(void);
extern time_to_full_charge can_get_time_to_charge(void);
extern void can_update_data(void *notNeededParameter);
extern bool can_is_charging(void);
extern void can_setup(void);
extern void can_read_data(void);

#endif