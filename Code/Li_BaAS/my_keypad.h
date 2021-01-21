#ifndef MY_KEYPAD_H
#define MY_KEYPAD_H

#define ADD_CRED_KEY        ('A')
#define DONE_KEY            ('D')
#define BACK_KEY            ('B')
#define CLR_KEY             ('C')

#define ADD_KEY_TIMEOUT_MS  (2*1000)

extern uint8_t keypad_get_key(void);
extern bool keypad_is_add_credits_pressed(void);

#endif