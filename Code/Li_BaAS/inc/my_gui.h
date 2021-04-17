#ifndef MY_GUI_H
#define MY_GUI_H

extern char* dtostrf (float val, signed char width, unsigned char prec, char *sout);

extern void gui_update_for_default_mode(void);

extern void gui_update_for_charging_mode(void);

extern void gui_update_for_creds_expired_mode(void);

extern void gui_update_for_add_creds_mode(void);

extern void gui_update_for_add_creds_mode_failure(void);

extern void gui_update_based_on_mode(void *vParameters);

extern void gui_update_for_add_creds_mode_success(uint32_t credits_added);

#endif /* MY_GUI_H */