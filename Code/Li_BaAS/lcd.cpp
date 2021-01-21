#include "my_lcd.h"
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 41, en = 43, d4 = 52, d5 = 50, d6 = 48, d7 = 46;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void lcd_setup(void) {
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
  
  //lcd.setCursor(0, 1);
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
