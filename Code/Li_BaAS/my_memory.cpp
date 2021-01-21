#include "Arduino.h"
#include "EEPROM.h"
#include "my_memory.h"

#define DEBUG

static void debug_log(char *buff) {
#ifdef DEBUG
    Serial.println(buff);
#endif
}

#define ADDRESS_FOR_CREDITS             (0)
#define ADDRESS_FOR_CREDS_VALIDITY      (8)

#define VALID                           (true)
#define INVALID                         (false)

static inline void mem_update_creds_validity(bool is_valid) {
    (is_valid)? EEPROM.write(ADDRESS_FOR_CREDS_VALIDITY,CREDS_VALID):\
    EEPROM.write(ADDRESS_FOR_CREDS_VALIDITY,CREDS_INVALID);
}

credits_validity mem_is_valid_credits(void) {
    
    uint8_t byte_read = EEPROM.read(ADDRESS_FOR_CREDS_VALIDITY);
    if (byte_read == CREDS_VALID)
        return CREDS_VALID;
    else
        return CREDS_INVALID;
}

uint64_t mem_get_credits_in_mem(void) {
    char test_string[128] = {};
    debug_log("Inside mem_get_credits_in_mem");

    uint64_t credits_saved = 0;
    if (mem_is_valid_credits() == CREDS_VALID) {
        uint8_t single_byte = 0;
        for(uint8_t i = 0; i < 8; i++)  {
            single_byte = EEPROM.read((ADDRESS_FOR_CREDITS+i));
            credits_saved |= (single_byte << (i*8));
            sprintf(test_string,"Data read at Addr = %u is = %u credits_saved = %u",\
                (ADDRESS_FOR_CREDITS+i),single_byte,credits_saved);
            debug_log(test_string);
        }
    }
    return credits_saved;
}

void mem_update_credits_in_mem(uint64_t credits) {
    uint8_t single_byte = 0;
    char test_string[128] = {};

    sprintf(test_string,"Inside mem_update_credits_in_mem with credits = %u",credits);
    debug_log(test_string);

    for (int i = 0; i < 8 ; i++) {
        single_byte = ((credits) >> (i*8)) & (0xff);
        sprintf(test_string,"Writting %d at address %u",single_byte,(ADDRESS_FOR_CREDITS+i));
        debug_log(test_string);
        EEPROM.write((ADDRESS_FOR_CREDITS+i),single_byte);
    }
    mem_update_creds_validity(VALID);
}
