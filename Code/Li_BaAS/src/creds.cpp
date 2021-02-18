#include "Arduino.h"
#include "../inc/my_creds.h"
#include "../inc/my_lcd.h"
#include "../inc/my_gui.h"
#include "../inc/my_keypad.h"
#include "../inc/my_memory.h"
#include "../inc/my_can.h"
#include "../inc/my_load.h"
#include "../inc/config.h"

static uint64_t creds_diff_soc_to_creds(uint8_t current_soc, uint8_t prev_soc) {
    uint64_t current_credits = creds_get_available_credits();
    uint64_t batt_cap_consumed = 0;
    int8_t diff_soc = prev_soc - current_soc;
    if ( diff_soc >  0 ) {
        batt_cap_consumed =  (uint64_t) ( (float) BATTERY_CAP_IN_WHR * ( (float) diff_soc / 100));
        current_credits -= (batt_cap_consumed * WHR_PER_RS);
    }
    if ((int64_t)current_credits < 0) {
        // mark it for credits expired
        current_credits = 0;
    }
    return current_credits;
}

static uint8_t prev_soc = 0;

static bool is_creds_expired = false;

static uint64_t credits_remaining = 0;

static add_credits_status string_to_credits(char *buff, uint64_t* credits_added) {

    char string_rcvd[64] = {};
    sprintf(string_rcvd, "String Rcvd = %s credits_rcvd = %lu",buff,*credits_added);
    debug_log(string_rcvd);

    if ((strchr(buff,'*')!= NULL) || (strchr(buff,'#')!= NULL)) {
        debug_log("Since */# is ADD_CREDS_FAILED\n");        
        *credits_added = NULL;
        return ADD_CREDS_FAILED;
    }
    int8_t len = strlen(buff) - 1;
    uint64_t mul = 1;
    uint64_t sum = 0;
    while(len >= 0) {
        sum += ((buff[len]) - 48) * mul;
        mul = mul * 10;
        len--;
    }
    *credits_added = sum;

    if(*credits_added)
        is_creds_expired = false;

    sprintf(string_rcvd,"Credits_added = %lu",*credits_added);
    debug_log(string_rcvd);

    return ADD_CREDS_SUCCESS;
}

void creds_update_credits(uint64_t credits) {
    char string_rcvd[64] = {};
    sprintf(string_rcvd, "Credits added = %lu",credits);
    debug_log(string_rcvd);

    credits_remaining += credits;
    sprintf(string_rcvd, "Credits updated to = %lu",credits_remaining);
    debug_log(string_rcvd);

    mem_update_credits_in_mem(credits_remaining);
}

uint64_t creds_get_available_credits(void) {

    if (!(credits_remaining) && (mem_is_valid_credits() == CREDS_VALID))
        credits_remaining  = mem_get_credits_in_mem();
    return credits_remaining;
}

static void creds_string_clear_char(char *buff) {
    uint8_t len = strlen(buff);

    char string_rcvd[32] = {};
    if  (len != 0) {
       sprintf(string_rcvd,"Character removed = %c",buff[len-1]);
        debug_log(string_rcvd);
        buff[len-1] = 0;
    }

    debug_log("New String now: ");
    debug_log(buff);

    lcd_clear();
    gui_update_for_add_creds_mode();
    lcd_setCursor(2,0);
    lcd_print_str(buff);    

}

static void creds_string_add_char(char *buff, uint8_t keypressed) {
    char debug_string[256] = {};
    sprintf(debug_string, "Adding %c to string = %s",keypressed,buff);
    debug_log(debug_string);

    uint8_t len = strlen(buff);
    char clear_buff[20] = {};
    buff[len] = keypressed;
    lcd_setCursor(2,0);
    lcd_print_str(clear_buff);
    lcd_print_str(buff);
    
    sprintf(debug_string,"String updated = %s",buff);
    debug_log(buff);
}

static add_credits_status creds_verify_credits_string(char *buff, uint64_t* credits_added) {
    if (ADD_CREDS_SUCCESS == string_to_credits(buff,credits_added)) {
        creds_update_credits(*credits_added);
        return ADD_CREDS_SUCCESS;
    }
    debug_log("Verification fails");
    return ADD_CREDS_FAILED;
}

add_credits_status creds_get_credits_from_user(void) {

    debug_log("Inside creds_get_credits_from_user");

    gui_update_for_add_creds_mode();
    lcd_setCursor(2,0);
    uint32_t time_to_start = millis();
    uint8_t keypressed = NULL;
    char buff[32] = {};

    uint8_t clear_key_count = 0;
    uint8_t add_key_count = 0;
    uint8_t keys_count = 0;

    while (true)
    {    
        while((keypressed = keypad_get_key()) == NULL) {
            uint32_t cur_time = millis();
            if ( (cur_time - time_to_start) > KEY_TIMEOUT_MS) {
                debug_log("Enter Key Timed out!");
                return ADD_CREDS_BACK_TO_DEF;
            }
        }

        char debug_string[32] = {};
        sprintf(debug_string,"Key Pressed = %c",keypressed);
        debug_log(debug_string);

        switch (keypressed) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '0':
            case '*':
            case '#':
                debug_log("Number key pressed");
                sprintf(debug_string,"Number Key Pressed = %c",keypressed);
                debug_log(debug_string); 
                keys_count++;
                if (keys_count > MAX_DIGIT) {
                    debug_log("Enterd Digits more than 20 hence ignoring");
                    if (keys_count > KEY_ENTERED_COUNT)
                        debug_log("Enterd Digits more than 25, hence returning!");
                        return ADD_CREDS_FAILED;
                } else {
                    creds_string_add_char(buff, keypressed);
                }
                break;

            case ADD_CRED_KEY:
                add_key_count++;
                if (add_key_count > ADD_KEY_VIOLATE_COUNT) {
                    debug_log("ADD_KEY count violated!");
                    add_key_count = 0;
                    return ADD_CREDS_FAILED;
                }
                break;
            
            case CLR_KEY:
                if (strlen(buff) == 0) {
                    clear_key_count++;
                    if (clear_key_count > CLEAR_KEY_VIOLATE_COUNT) {
                        debug_log("CLR_KEY count violated!");
                        return ADD_CREDS_FAILED;
                    }
                } else {
                    creds_string_clear_char(buff);
                }
                break;

            case BACK_KEY:
                debug_log("BACK_KEY hence returning!");
                return ADD_CREDS_BACK_TO_DEF;
                break;
            
            case DONE_KEY:
                debug_log("Done key pressed started verification!");
                uint64_t creds_added;
                uint32_t status = creds_verify_credits_string(buff, &creds_added); 
                if (status == ADD_CREDS_SUCCESS) {
                    debug_log("Verificaiton successfull!");
                    gui_update_for_add_creds_mode_success(creds_added);
                    delay(1000);
                    return ADD_CREDS_SUCCESS;
                } else if (status == ADD_CREDS_FAILED) {
                    debug_log("Verificaiton Failed!");
                    bool timed_out = false;
                    uint32_t failed_at_ms = millis();
                    gui_update_for_add_creds_mode_failure();
                    debug_log("Waiting for back_key.........");
                    while (timed_out == false)
                    {
                        keypressed = keypad_get_key();
                        if (keypressed == BACK_KEY) {
                            debug_log("Back key pressed after failure!");
                            return creds_get_credits_from_user();
                        }
                        else 
                            timed_out = (millis() - failed_at_ms) > KEY_TIMEOUT_MS;
                    }
                    if (timed_out) {
                        debug_log("Waited for 1 min and timed out hence returning!");
                        return ADD_CREDS_BACK_TO_DEF;
                    }
                }
                break;

            default:
                break;
        }
    }
}

bool creds_is_creds_expired(void) {
    return is_creds_expired;
}

void creds_update_creds_based_on_soc(void) {
    uint8_t current_soc = can_get_soc();
    uint64_t current_credits = creds_get_available_credits();
    if ((current_soc < prev_soc) && !(creds_is_creds_expired()) && credits_remaining) {
        current_credits -= (uint64_t)(prev_soc - current_soc);
        credits_remaining = current_credits;
        mem_update_credits_in_mem(credits_remaining);
    }
    if (credits_remaining == 0) {
        is_creds_expired = true;
        mem_update_credits_in_mem(credits_remaining);
        load_disable_discharge();
    }else
        load_enable_discharge();
    prev_soc = current_soc;
}