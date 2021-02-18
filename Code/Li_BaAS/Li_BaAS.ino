#include "inc/my_lcd.h"
#include "inc/my_task.h"
#include "inc/my_can.h"
#include "inc/my_led.h"
#include "inc/my_load.h"

void setup() {
  Serial.begin(9600);
  led_setup();
  load_setup();
  lcd_setup();  
  task_init();
  can_setup();
}

void loop() {
  // Nothing to do
}
