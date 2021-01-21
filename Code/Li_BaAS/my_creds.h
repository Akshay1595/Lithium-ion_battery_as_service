#ifndef MY_CREDS_H
#define MY_CREDS_H

typedef enum {
    ADD_CREDS_FAILED,
    ADD_CREDS_SUCCESS,
    ADD_CREDS_BACK_TO_DEF
}add_credits_status;

#define KEY_TIMEOUT_MS              (60000UL)
#define ADD_KEY_VIOLATE_COUNT       (5)
#define CLEAR_KEY_VIOLATE_COUNT     (5)
#define MAX_DIGIT                   (20)
#define KEY_ENTERED_COUNT           (25)


extern void creds_update_credits(void);

extern uint64_t creds_get_available_credits(void);

extern add_credits_status creds_get_credits_from_user(void);

extern void gui_update_for_add_creds_mode_success(uint32_t credits_added);

extern void gui_update_for_add_creds_mode_failure(void);


#endif /* MY_CREDS_H */