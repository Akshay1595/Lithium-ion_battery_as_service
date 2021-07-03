#include "Arduino.h"
#include "../inc/my_lcd.h"
#include <LiquidCrystal.h>
#include "../inc/config.h"

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
//const int rs = 41, en = 43, d4 = 52, d5 = 50, d6 = 48, d7 = 46;

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void lcd_setup(void) {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  lcd.setCursor(7, 0);
  lcd_print_str("Welcome");
  
  lcd.setCursor(9, 1);
  lcd_print_str("To");

  lcd.setCursor(7, 2);
  lcd_print_str("LiBaas");

  lcd.setCursor(7, 3);
  lcd_print_str("System");

  debug_log("lcd_setup done!");
}

void lcd_print_str(char* buff) {
	lcd.print(buff);
}

void lcd_print_char(char char_to_print) {
  lcd.print(char_to_print);
}

void lcd_setCursor(int row,int column) {
  lcd.setCursor(column, row);
}

void lcd_clear(void) {
  lcd.clear();
}
