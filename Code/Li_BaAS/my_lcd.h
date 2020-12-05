#ifndef MY_LCD_H
#define MY_LCD_H

void lcd_print_str(char* buff);
void lcd_setup(void);
void lcd_setCursor(int row,int column);
void lcd_print_char(char char_to_print);
void lcd_clear(void);

#endif
