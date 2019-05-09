# SerialVariables Arduino Library

This library allows you to create and retrieve variables that can be modified via serial commands.

[TOC]

## Installation

Download this project as a zip file and [add it via the Arduino IDE](https://www.arduino.cc/en/guide/libraries#toc4).

## Considerations

Please make sure to include both New Line (`\n`) and a Carriage Return (`\r`) characters in your serial message for the library to know when a command has ended.

In the Arduino IDE, this can be done by selecting "Both NL & CR" in the Serial Monitor window.

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

During runtime, you can update the value of `mySerialVariable` by writing through the serial monitor `mySerialVariable=1`.

## Methods

### setAssigner(char character) : void

Sets `character` as the assigner for the library to know when a variable name ends the value starts.

This function should only be called in `setup()`.

By default, the value of the assigner is an equals sign (`=`).

### setVerbose(bool flag) : void

If `flag = true`, then the library will output normal, warning, and error messages when something happens. Useful when debugging.

The default value for verbosity is `false`.

(Also I think it could use a better name, I think verbosity is not quite the right word).

### handle() : void

Updates the internal library buffer and tries to execute commands when a message is sent.

It **MUST** always be included only once in the loop.

### addInt(String name, int value=0) : void

Adds a new integer serial variable with a `name` and a default `value` of 0.

Should only be called on `setup()`.

### setInt(String name, int value) : void

Sets the `value` of an integer serial valuable called `name`.

### getInt(String name) : int

Returns the value of the variable `name`.

Returns zero (0) if a variable of said `name` doesn't exist.
