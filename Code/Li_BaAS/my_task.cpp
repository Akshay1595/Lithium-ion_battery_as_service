#include "Arduino.h"
#include <Arduino_FreeRTOS.h>
#include "my_task.h"
#include "my_keypad.h"
#include "my_lcd.h"
#include "my_gui.h"
#include "my_creds.h"

#define PRIORITY_HIGH       (2)
#define PRIORITY_MED        (1)
#define PRIORITY_LOW        (0)

#define DEBUG

static void debug_log(char *buff) {
#ifdef DEBUG
    Serial.println(buff);
#endif
}

static bool add_key_pressed = false;
TaskHandle_t handle_for_task_to_accept_creds_from_user;

void task_to_check_if_add_key_pressed(void *pvParameters) {
  while (true)
  {
    //debug_log("Inside task task_to_check_if_add_key_pressed");
    if (keypad_is_add_credits_pressed() == true) {
        debug_log("ADD_KEY is pressed\n");
        add_key_pressed = true;
        delay(50);
        vTaskResume(handle_for_task_to_accept_creds_from_user);
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }  
}

void task_to_print_lcd_data(void *pvParameters) {
    while (true) {
      debug_log("--- Inside task task_to_print_lcd_data");
      //gui_update_for_creds_expired_mode();
      gui_update_for_charging_mode();
      //gui_update_for_default_mode();
      delay(10);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task_to_accept_creds_from_user(void *pvParameters) {
    //suspend the task initially
    vTaskSuspend(handle_for_task_to_accept_creds_from_user);
    
    while (true) {
      debug_log("### Inside task task_to_accept_creds_from_user");
      debug_log("Resumed Suspended Task!");
      if (add_key_pressed == true)  {
        debug_log("Seems ADD_KEY is pressed! Need to accept user credits");
        creds_get_credits_from_user();
        debug_log("Came back from accepting The credits!\n");
        add_key_pressed = false;
        debug_log("Suspending task again!");
        vTaskSuspend(handle_for_task_to_accept_creds_from_user);
      }
    }
}

void task_init(void) {
  
  xTaskCreate(task_to_accept_creds_from_user, "TASK_TO_ACCEPT_CREDS_FROM_USER", 1024, NULL, PRIORITY_HIGH, &handle_for_task_to_accept_creds_from_user);

  xTaskCreate(task_to_print_lcd_data, "TASK_TO_PRINT_LCD_DATA", 1024, NULL, PRIORITY_MED, NULL); 
  
  xTaskCreate(task_to_check_if_add_key_pressed, "TASK_TO_CHECK_IF_ADD_KEY_PRESSED", 1024, NULL, PRIORITY_LOW, NULL);

}
