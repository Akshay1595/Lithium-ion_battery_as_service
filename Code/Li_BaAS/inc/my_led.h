#ifndef LED_H
#define LED_H

extern void led_setup(void);

extern void led_toggle_can_data_led(void);

extern void led_set_load_led(void);

extern void led_clear_load_led(void);

extern void led_toggle_gui_task_led(void);

extern void led_set_charging_indicator(void);

extern void led_clear_charging_indicator(void);

#endif // LED_H