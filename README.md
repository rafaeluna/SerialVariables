# SerialVariables Arduino Library

This library allows you to create and retrieve variables that can be modified via serial commands.

## Installation

Download this project as a zip file and [add it via the Arduino IDE](https://www.arduino.cc/en/guide/libraries#toc4).

## Basic Usage

This is the code of the most basic example I could think of.

```c++
// Include the library in your sketch
#include <SerialVariables.h>

// The library instantiates an object for you, like the Wire library

void setup() {

	// Begin your serial communication, any baudrate will do
	Serial.begin(9600);

	// Add a new serial variable, this time it will be an int.
	// You must specify the name of the variable,
	// and you can optionally add an initial value
	SerialVariables.addInt("mySerialVariable", 50);

}

void loop() {
	// Make a call for handling the serial variables.
	// This must always be specified once in the loop
	SerialVariables.handle();

	// Get the int serial variable and store it and an actual variable
	int myNormalVariable = SerialVariables.getInt("mySerialVariable");
	
	// Print the value
	Serial.println(myNormalVariable);
}
```