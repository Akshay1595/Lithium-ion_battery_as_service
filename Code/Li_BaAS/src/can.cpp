#include "Arduino.h"
#include <SPI.h>
#include <mcp2515.h>
#include "../inc/my_creds.h"
#include "../inc/my_can.h"
#include <Arduino_FreeRTOS.h>
#include "../inc/config.h"
#include "../inc/my_led.h"

#define DISCHARGE_RATE_MAX      (5000)
#define CHARGE_RATE_MAX         (2000)

#define CS_PIN                  (53)

MCP2515 mcp2515(CS_PIN);

static uint8_t SOC = 100;
static uint32_t discharge_rate = DISCHARGE_RATE_MAX;
static uint32_t charge_rate = CHARGE_RATE_MAX;
static time_to_full_charge time_remaining = {2, 30};
static bool is_charger_connected = false;

bool can_charger_connected(void) {
    return is_charger_connected;
}

void can_setup(void) {
    mcp2515.reset();
    mcp2515.setBitrate(CAN_250KBPS);//CAN_500KBPS
    mcp2515.setNormalMode();
    debug_log("Can Setup complete!");
}

void can_read_data(void) {
    struct can_frame canMsg;
    //Update CAN data
    char msg_string[32] = {};
    char add_string[16] = {};
    while ((mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK))  {

        canMsg.can_id = canMsg.can_id & ( (1U << 30) - 1);

        Serial.print(canMsg.can_id, HEX); // print ID
        Serial.print(" "); 
        Serial.print(canMsg.can_dlc, HEX); // print DLC
        Serial.print(" ");
        
        for (int i = 0; i < canMsg.can_dlc; i++)  {  // print the data
            Serial.print(canMsg.data[i],HEX);
            Serial.print(" ");
        }
        Serial.println(" ");
        
    }
}

void can_update_data(void *pvParameters) {
    while (true){

        debug_log("Inside the task can_task_to_update_soc");

        // Update SOC
        if (!is_charger_connected){
            if ( (!creds_is_creds_expired())) {
                if (SOC != 0)
                    SOC = SOC - 1;
                else
                    is_charger_connected = true;                    
            } 
        } else {
            if (SOC < 100)
                SOC = SOC + 1;
            else
                is_charger_connected = false;
        }

        // Update Discharge Rate
        {
            if (!is_charger_connected && !(creds_is_creds_expired())) {
                if( discharge_rate == 0)
                    discharge_rate = DISCHARGE_RATE_MAX;
                else
                    discharge_rate -= 10;
            }
            else
                discharge_rate = 0;
        }

        //Update charger connection
        {
            if(is_charger_connected) {
                if (charge_rate == 0) 
                    charge_rate = CHARGE_RATE_MAX;
                else
                    charge_rate -= 100;
            }
        }
        led_toggle_can_data_led();
        //debug_log("Returning from this can data update function!\n");
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
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