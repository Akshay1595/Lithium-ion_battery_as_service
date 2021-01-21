#include "Arduino.h"
#include <Keypad.h>
#include "my_keypad.h"

const byte numRows= 4;
const byte numCols= 4;

unsigned char keymap[numRows][numCols]= { {'1', '2', '3', ADD_CRED_KEY},

{'4', '5', '6', BACK_KEY},

{'7', '8', '9', CLR_KEY},

{'*', '0', '#', DONE_KEY} };

byte rowPins[numRows] = {33,31,29,27}; //Rows 0 to 3

byte colPins[numCols]= {32,30,28,26}; //Columns 0 to 3

//initializes an instance of the Keypad class

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

uint8_t keypad_get_key(void) {
	uint8_t keypressed = myKeypad.getKey();
	if (keypressed != NO_KEY)
		return keypressed;
	else
		return NULL;
}


bool keypad_is_add_credits_pressed(void) {
	uint8_t keypressed = myKeypad.getKey();
	uint32_t start_time,cur_time;
	if (keypressed == ADD_CRED_KEY) {
/*
		start_time = millis();
		while ((cur_time - start_time) < ADD_KEY_TIMEOUT_MS) {
			cur_time = millis();
		}
		keypressed = myKeypad.getKey();
		if (keypressed == ADD_CRED_KEY) {
*/			return true;
//		}
	}
	return false;
}
