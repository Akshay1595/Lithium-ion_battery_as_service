#include <Keypad.h>
#include "my_keypad.h"

const byte numRows= 4;

const byte numCols= 4;

unsigned char keymap[numRows][numCols]= { {'1', '2', '3', 'A'},

{'4', '5', '6', 'B'},

{'7', '8', '9', 'C'},

{'*', '0', '#', 'D'} };

byte rowPins[numRows] = {13,12,11,10}; //Rows 0 to 3

byte colPins[numCols]= {9,8,7,6}; //Columns 0 to 3

//initializes an instance of the Keypad class

Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

void keypad_setup(void) {	
}

//If key is pressed, this key is stored in 'keypressed' variable //If key is not equal to 'NO_KEY', then this key is printed out //if count=17, then count is reset back to 0 (this means no key is pressed during the whole keypad scan process

char keypad_get_key(void) {

	char keypressed = myKeypad.getKey();

	if (keypressed != NO_KEY)
		return keypressed;
	else
		return NULL;
}
