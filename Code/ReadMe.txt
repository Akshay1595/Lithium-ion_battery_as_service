LiBaAs Code Details:

1. Code is FreeRTOS based running on Arduino MEGA board.
2. Required Libraries are CAN MCP2515 Libraries of Arduino, LCD libraries, Keypad Library, FreeRTOS library.
3. Environment setup used is Visual studio with Arduino toolbox

LiBaAs Code Structure:

1. Based on FreeRTOS we have number of tasks created for this particular project. Each task has individual assigned activities to perform.
For ex. Displaying data, reading CAN bus messages and updating global data, Check if ADD key is pressed, operation to perform ADD key is pressed
2. Detailed operation to be performed for each task can be viewed through code.


Libraries and their links:

1. MCP2515 Library: https://github.com/autowp/arduino-mcp2515
	YouTube video for referrence: https://www.youtube.com/watch?v=2gnXKMoFwkc
2. FreeRTOS Library: Install lib from location given below:
	https://github.com/feilipu/Arduino_FreeRTOS_Library
3. KeyPad library:
	https://playground.arduino.cc/uploads/Code/keypad/index.zip
	Brief info about it:
	https://playground.arduino.cc/Code/Keypad/