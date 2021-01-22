#include <math.h>
#include "Arduino.h"
#include "my_gui.h"
#include "my_lcd.h"
#include "my_can.h"
#include "my_creds.h"


static void get_credits_to_watt_in_string(uint64_t credits, char *buff) {
    char string_c[20] = {};
    if (credits < 1000) {
      sprintf(buff, "Creds: %u W",credits);
    } else if (credits < 1000000) {
      dtostrf(((float)credits/1000),5,2,string_c);
      sprintf(buff, "Creds: %s KW",string_c);     
    }
    else {
      dtostrf(((float)credits/1000000),5,2,string_c);
      sprintf(buff, "Creds: %s MW",string_c);      
    }
}

static uint32_t my_pow(uint8_t number,uint8_t index) {
    uint32_t final = 1;
    while (index) {
        final *= number;
        index--;
    }
    return final;
}

static inline void get_credits_to_string(char* buff) {
    get_credits_to_watt_in_string(creds_get_available_credits(),buff);
}

static void get_soc_to_string(char *buff) {
    uint8_t soc = can_get_soc();
    sprintf(buff, "Battery: %d %%",soc);
}

static char *dtostrf (float val, signed char width, unsigned char prec, char *sout) {
  uint16_t iPart = (uint16_t)val;
  uint32_t dPart = (uint32_t)((float)my_pow(10,2) * (val - (float)iPart));
  sprintf(sout, "%d.%d", iPart, dPart);  
  return sout;
}


static void gui_print_lcd(char lcd_string[4][20]) {
    lcd_clear();
    lcd_setCursor(0, 0);
    lcd_print_str(lcd_string[0]);
    lcd_setCursor(1, 0);
    lcd_print_str(lcd_string[1]);
    lcd_setCursor(2, 0);
    lcd_print_str(lcd_string[2]);
    lcd_setCursor(3, 0);
    lcd_print_str(lcd_string[3]);

}

void gui_update_for_default_mode(void) {

    char lcd_string[4][20] = {};
    get_credits_to_string(lcd_string[0]);
    get_soc_to_string(lcd_string[2]);

    uint32_t dis_rate = can_get_discharge_rate();
    sprintf(lcd_string[1], "Discharge: %dW",dis_rate);

    gui_print_lcd(lcd_string);
}

void gui_update_for_charging_mode(void) {
    char lcd_string[4][20] = {};
    get_soc_to_string(lcd_string[3]);
    
    time_to_full_charge time_remaining = can_get_time_to_charge();
    uint32_t charge_rate = can_get_charge_rate();
    
    sprintf(lcd_string[0], "%dHr %dMin to charge",time_remaining.hrs,time_remaining.mins);
    sprintf(lcd_string[1], "Charging at %dW",charge_rate);
    
    get_credits_to_string(lcd_string[2]);
    
    gui_print_lcd(lcd_string);
}

void gui_update_for_creds_expired_mode(void) {

    char lcd_string[4][20] = {};
    get_soc_to_string(lcd_string[3]);
    get_credits_to_string(lcd_string[0]);
    strcpy(lcd_string[1],"Credits Expired");
    strcpy(lcd_string[2],"Add Credits");

    gui_print_lcd(lcd_string);
}

void gui_update_for_add_creds_mode(void) {
    char lcd_string[4][20] = {};
    get_credits_to_string(lcd_string[3]);
    strcpy(lcd_string[0],"Enter Credits and");
    strcpy(lcd_string[1],"press Done");
    gui_print_lcd(lcd_string);
}

void gui_update_for_add_creds_mode_success(uint32_t credits_added) {
    char lcd_string[4][20] = {};
    strcpy(lcd_string[0],"Success");
    strcpy(lcd_string[1],"Credits added");

    get_credits_to_watt_in_string(credits_added,lcd_string[2]);

    get_credits_to_string(lcd_string[3]);
    gui_print_lcd(lcd_string);
}

void gui_update_for_add_creds_mode_failure(void) {
    char lcd_string[4][20] = {};

    strcpy(lcd_string[0],"Failure");
    strcpy(lcd_string[1],"Press Back and");
    strcpy(lcd_string[2],"Re-enter credits");

    get_credits_to_string(lcd_string[3]);
    gui_print_lcd(lcd_string);
}
