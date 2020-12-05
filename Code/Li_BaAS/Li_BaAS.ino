#include "my_keypad.h"
#include "my_lcd.h"


char string[32] = {};
char keypressed = NULL;
unsigned char soc_percentage = 100;
uint32_t credits_remaining = 1000;

void string_to_credits(char *buff) {
  int8_t len = strlen(buff) - 1;
  uint32_t mul = 1;
  uint32_t sum = 0;
  while(len >= 0) {
    sum += ((buff[len]) - 48) * mul;
    mul = mul * 10;
    len--;
  }
  credits_remaining = sum;
}

uint8_t get_soc(void) {
  return 100;
}

uint32_t get_credits(void) {
  return credits_remaining;
}

void print_soc_string() {
  soc_percentage = get_soc();
  sprintf(string, "SOC=%d",soc_percentage);
  lcd_setCursor(0, 0);
  lcd_print_str(string);lcd_print_char('%');
}

void print_credits_string() {
  credits_remaining = get_credits();
  lcd_setCursor(1, 0);
  sprintf(string,"Creds=%d",credits_remaining);
  lcd_print_str(string);
}

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  keypad_setup();
  lcd_setup();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((keypressed = keypad_get_key()) != NULL) {
    if  (keypressed == '*') {
      lcd_clear();
      lcd_setCursor(0, 0);
      lcd_print_str("Enter Credits");
      lcd_setCursor(1, 0);
      memset(string,0,32);
      uint8_t count = 0;
      while(count < 4) {
        if ((keypressed = keypad_get_key()) != NULL) {
          if(keypressed == '#')
              break;
          string[count] = keypressed;
          lcd_print_char(keypressed);
          count++;
        }
      }
      delay(2000);
      string_to_credits(string);
      lcd_setCursor(0, 0);
      lcd_print_str("Received Credits");
      delay(2000);
      lcd_clear();
    }
  }
  print_soc_string();
  print_credits_string();
}
