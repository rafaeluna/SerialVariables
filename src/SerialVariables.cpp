#include "SerialVariables.h"


// Initialize class variables
Mode			Serial_Variables::mode = ModeNormal;
HardwareSerial*	Serial_Variables::hardwareSerial;
SoftwareSerial*	Serial_Variables::softwareSerial;
char			Serial_Variables::assigner = '=';
bool			Serial_Variables::verbose = false;
uint8_t			Serial_Variables::numberOfVariables = 0;
var				Serial_Variables::variables[MAX_VARIABLES];
String			Serial_Variables::buffer = "";
bool			Serial_Variables::finishedReading = false;


// Constructors ////////////////////////////////////
Serial_Variables::Serial_Variables() {

}


// Public Methods //////////////////////////////////


void Serial_Variables::useHardwareSerial(HardwareSerial serial) {
	hardwareSerial = &serial;
	mode = ModeHardware;
}

void Serial_Variables::useSoftwareSerial(SoftwareSerial serial) {
	softwareSerial = &serial;
	mode = ModeSoftware;
}

void Serial_Variables::setAssigner(char character) {
	/*
	 * Sets a new assigner character
	 */
	assigner = character;
}

void Serial_Variables::setVerbose(bool flag /*=true*/) {
	/*
	 * Sets verbosity of messages emitted by the library
	 */
	verbose = flag;
}

void Serial_Variables::handle() {
	/*
	 * Updates buffer and processes it when ready
	 */
	updateBuffer();
	if (finishedReading) processBuffer();	
}

void Serial_Variables::addInt(String name, int value /*=0*/) {
	/*
	 * "Declares" an integer serial variable
	 */
	addVariable(var(TypeInt, name, String(value)));
}

void Serial_Variables::setInt(String name, int value) {
	/*
	 * Sets the value of an integer variable
	 */
	int foundIndex = findIndex(name);
	if (foundIndex<0) {
		printError("in setInt(): variable name not found");
		return;
	}
	variables[foundIndex].value = String(value);
}

int Serial_Variables::getInt(String name) {
	/*
	 * Returns the value of an integer variable
	 * with a given name
	 */
	int foundIndex = findIndex(name);

	if (foundIndex<0) {
		printError("in getInt(): variable name not found");
		return 0;
	}

	int value = variables[foundIndex].value.toInt();

	if (variables[foundIndex].type != TypeInt) {
		printWarning("in getInt(): variable was not declared as int");
		printWarning("attempt to cast as int: "+value);
	}
	return value;
}


// Private Methods /////////////////////////////////

void Serial_Variables::updateBuffer() {
	/*
	 * Updates internal buffer from serial
	 */
	if (Serial.available()) {
		char reading = Serial.read();
		if (reading == '\n') finishedReading = true;
		else if (reading != '\r') buffer += reading;
	}
}

void Serial_Variables::processBuffer() {
	/*
	 * Processes the buffer data
	 */

	// Get a copy of buffer and clear it
	const String message = buffer;
	resetBuffer();

	// Display value if required by user
	if (message.endsWith("?")) {

		String assumedName = message.substring(0,message.length()-1);
		assumedName.trim();
		int variableIndex = findIndex(assumedName);

		if (!isIndexValid(variableIndex)) {
			printError("\""+ assumedName + "\"" + "variable not found");
			return;
		}

		printMessage(assumedName + " = " + variables[variableIndex].value);
		return;
	}

	// Find assigner within message
	int assignerIndex = findAssignerIndex(message);
	if (assignerIndex < 0) return;

	// Find index of variable given its name
	String assumedName = message.substring(0,assignerIndex);
	assumedName.trim();
	int variableIndex = findIndex(assumedName);
	if (!isIndexValid(variableIndex)) return;

	// Set variable's new value
	String assumedValue = message.substring(assignerIndex+1);
	setValue(variableIndex,assumedValue);
}

void Serial_Variables::resetBuffer() {
	/*
	 * Clears internal buffer and sets up a new reading
	 */
	finishedReading = false;
	buffer = "";
}

int Serial_Variables::findAssignerIndex(String str) {
	/*
	 * Finds an assigner in a string and returns it's index
	 */
	int assignerIndex = str.indexOf(assigner);

	// Error handling
	if (assignerIndex == -1) {
		printError("No assigner \""+String(assigner)+"\"");
	}
	else if (assignerIndex == 0) {
		printError("No variable name");
		assignerIndex = -1;
	}
	else if (assignerIndex == str.length()-1) {
		printError("No variable value");
		assignerIndex = -1;
	}
	return assignerIndex;
}

int Serial_Variables::findIndex(String name) {
	/*
	 * Finds a variable given it's name and
	 * returns it's index
	 */
	for (int i=0; i<numberOfVariables; i++) {
		if (variables[i].name==name) return i;
	}
	printError("\""+ name + "\" variable not found");
	return -1;
}

void Serial_Variables::setValue(int variableIndex, String assumedValue) {
	/*
	 * Sets the value of a given variable given its index, type
	 * and an assumed value (to be checked)
	 */

	const Type type = variables[variableIndex].type;

	switch (type) {

		case TypeInvalid:
			return;

		case TypeInt: {
			int value = assumedValue.toInt();
			// If conversion is zero and it was not intended, return error
			if (value==0 && assumedValue!="0") {
				printError("\""+assumedValue+"\" is not an integer");
				return;
			}
			if (assumedValue.indexOf('.')>=0) printWarning("Casted float to int");
			variables[variableIndex].value = value;
			printMessage(variables[variableIndex].name+" set to: "+value);
			break;
		}

		// Add more cases
	}
}

void Serial_Variables::addVariable(var variable) {
	/*
	 * Adds a variable to the array
	 * checks for out of bounds index
	 */

	if (numberOfVariables >= MAX_VARIABLES) {
		printError("Maximum number of variables reached");
		return;
	}

	variables[numberOfVariables] = variable;
	numberOfVariables++;

	if (numberOfVariables >= MAX_VARIABLES) {
		printWarning("Maximum number of variables reached");
	}
}

bool Serial_Variables::isIndexValid(int index) {
	/*
	 * Checks if an index is within accectable range
	 * [0, MAX_VARIABLES)
	 */
	return (0<=index<MAX_VARIABLES);
}

void Serial_Variables::printError(String errorMessage) {
	/*
	 * Stylistically outputs an error with a given message
	 */
	Serial.println();
	if (!verbose) return;
	Serial.println(String("ERROR: ")+errorMessage);
}

void Serial_Variables::printWarning(String warningMessage) {
	/*
	 * Stylistically outputs a warning with a given message
	 */
	Serial.println();
	if (!verbose) return;
	Serial.println(String("WARNING: ")+warningMessage);
}

void Serial_Variables::printMessage(String message) {
	/*
	 * Stylistally outputs a simple message
	 */
	Serial.println();
	if (!verbose) return;
	Serial.println(message);
}

// Preinstantiate Objects //////////////////////////

class Serial_Variables SerialVariables = Serial_Variables();

#undef MAX_VARIABLES