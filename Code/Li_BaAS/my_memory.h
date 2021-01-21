#ifndef MY_MEMORY_H
#define MY_MEMORY_H

typedef enum  {
    CREDS_INVALID = 0xFF,
    CREDS_VALID = 0
} credits_validity;


extern void mem_update_credits_in_mem(uint64_t credits);

extern uint64_t mem_get_credits_in_mem(void);

extern credits_validity mem_is_valid_credits(void);

#endif
