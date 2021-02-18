#include "my_lcd.h"
#include "my_task.h"
#include "my_can.h"
#include "my_led.h"
#include "my_load.h"

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
