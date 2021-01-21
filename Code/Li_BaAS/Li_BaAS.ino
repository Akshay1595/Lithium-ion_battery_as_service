#include "my_lcd.h"
#include "my_task.h"

void setup() {
  Serial.begin(9600);
  lcd_setup();  
  task_init();
}

void loop() {
  // Nothing to do
}
