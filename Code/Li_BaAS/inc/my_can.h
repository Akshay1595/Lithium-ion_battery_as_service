#ifndef MY_CAN_H
#define MY_CAN_H

typedef struct time_t {
    uint8_t hrs;
    uint8_t mins;
}time_to_full_charge;

extern uint8_t can_get_soc(void);
extern uint32_t can_get_discharge_rate(void);
extern uint32_t can_get_charge_rate(void);
extern time_to_full_charge can_get_time_to_charge(void);
extern void can_update_data(void *notNeededParameter);
extern bool can_charger_connected(void);
extern void can_setup(void);
extern void can_read_data(void);

#endif