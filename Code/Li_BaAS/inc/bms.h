#ifndef BMS_H
#define BMS_H

// types to use
typedef bool status_of_charging;
typedef float discharge_rate;
typedef bool status_of_charger_connected;

//---------------------------------------------------------------------------------//
/* BMS Information Message 1*/
#define BMS_INFO_MSG_1_MSG_ID       (0x18FF28F4)

// Fault level 
#define NO_FAULT                    (00)
#define FAULT_LEVEL_1               (01)
#define FAULT_LEVEL_2               (02)
#define FAULT_LEVEL_3               (03)

#define CHARGING_ON                 (true)
#define CHARGING_OFF                (false)

#define CHARGER_CONNECTED           (true)
#define CHARGING_DISCONNECTED       (false)

//  BMS information message byte 0
typedef struct bms_info_1 {
    uint8_t is_charger_connected:1;
    uint8_t is_charging_on:1;
    uint8_t is_batt_power_loss:1;
    uint8_t is_batt_pack_ready:1;
    uint8_t is_contactor_open:1;
    uint8_t reserved:3;
}bms_basic_info_byte1;

//  BMS information message byte 1
typedef uint8_t state_of_charge;

//  BMS information message byte 2 and 3
typedef float batt_i;
#define INVALID_CURRENT_VALUE       (1000.0f)

typedef struct charge_discharge_current {
    uint8_t i_low;
    uint8_t i_high;
}bms_charge_discharge_current_struct;
// union to access discharge current value 
typedef union u_dis_curr{
    bms_charge_discharge_current_struct current;
    int16_t bms_current;
}u_bms_charge_discharge_current;

// BMS information message byte 4 and 5
typedef float batt_v;
#define INVALID_VOLTAGE_VALUE       (1000.0f)

typedef struct battery_voltage {
    uint8_t batt_vltg_low;
    uint8_t batt_vltg_high;
}bms_batt_vltg_struct;
// union to access the battery_voltage
typedef union batt_vltg_u{
    bms_batt_vltg_struct vltg_st;
    uint16_t batt_vltg;
}u_bms_batt_vltg;

// BMS information message byte 6
typedef uint8_t batt_failure_level;

// BMS information message byte 7
typedef uint8_t error_code;

typedef struct information_msg_1 {
    bms_basic_info_byte1 byte1;
    state_of_charge soc;
    u_bms_charge_discharge_current current;
    u_bms_batt_vltg batt_vltg;
    batt_failure_level failure_level;
    error_code bms_error_code;
}bms_info_msg_1;

//---------------------------------------------------------------------------------//
/* BMS Information Message 2*/
//msg ID
#define BMS_INFO_MSG_2_MSG_ID       (0x18FE28F4)

// bms information message byte 1
typedef uint8_t highest_cell_vltg_low;
// bms information message byte 2
typedef uint8_t highest_cell_vltg_high;
// highest cell voltage among all the cells
typedef struct highest_cell_vltg_t  {
    highest_cell_vltg_low low_byte;
    highest_cell_vltg_high high_byte;
}highest_cell_vltg_struct;
// union to access highest cell vltg
typedef union highest_cell_vltg_u {
    highest_cell_vltg_struct vltg;
    uint16_t highest_cell_vltg;
}u_highest_cell_vltg;

// bms information message byte 3
typedef uint8_t lowest_cell_vltg_low;
// bms information message byte 4
typedef uint8_t lowest_cell_vltg_high;
// bms lowest_cell_vltg
typedef struct lowest_cell_vltg_st{
    lowest_cell_vltg_low low_byte;
    lowest_cell_vltg_high high_byte;
}lowest_cell_vltg_struct;
// union to access lowest cell voltage
typedef union lowest_cell_vltg_u  {
    lowest_cell_vltg_struct low_vltg_st;
    uint16_t lowest_cell_vltg;
}u_lowest_cell_vltg;

// bms information message byte 5
typedef uint8_t max_temp;
// bms information message byte 6
typedef uint8_t min_temp;

// bms information message byte 7
typedef uint8_t max_dischrge_current_low;
// bms information message byte 8
typedef uint8_t max_dischrge_current_high;
// struct for max_discharge_current
typedef struct max_dis_current_st {
    max_dischrge_current_low low_byte;
    max_dischrge_current_high high_byte;
}max_dischrge_current_struct;
// union to access max_discharge_current
typedef union max_dis_current_u {
    max_dischrge_current_struct dis_current_st;
    int16_t max_discharge_current;
}u_max_dis_current;

// BMS global data
typedef struct bms_data_t {
    state_of_charge soc;
    batt_i current;
    batt_v batt_vltg;
    bool is_charging;
    bool is_fault;
}bms_data;

// functions
void bms_decrypt_can_msg(struct can_frame* canMsg);
extern discharge_rate bms_get_discharge_rate(void);
extern state_of_charge bms_get_soc(void);
status_of_charging bms_is_charging(void);
#endif //end of BMS_H