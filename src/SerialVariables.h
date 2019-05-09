#ifndef SERIALVARIABLES_H
#define SERIALVARIABLES_H

#include "Arduino.h"
#include "SoftwareSerial.h"

#define MAX_VARIABLES 16

enum Type {
	TypeInvalid = -1,	
	TypeInt,
	TypeFloat,
	TypeDouble,
	TypeString
};

struct var {
	Type type;
	String name;
	String value;
	var(Type t=TypeInvalid, String n="VAR", String v="0"): type(t), name(n), value(v) {}
};

enum Mode {
	ModeNormal = 0,
	ModeHardware,
	ModeSoftware
};

class Serial_Variables {
	private:
		static Mode mode;
		static HardwareSerial *hardwareSerial;
		static SoftwareSerial *softwareSerial;
		static char assigner;
		static bool verbose;
		static uint8_t numberOfVariables;
		static var variables[MAX_VARIABLES];
		static String buffer;
		static bool finishedReading;
		static void updateBuffer();
		static void processBuffer();
		static void resetBuffer();
		static int findAssignerIndex(String str);
		static int findIndex(String assumedName);
		static void setValue(int variableIndex, String assumedValue);
		static void addVariable(var variable);
		static void printError(String errorMessage);
		static void printWarning(String warningMessage);
		static void printMessage(String message);
		static bool isIndexValid(int index);
	public:
		Serial_Variables();
		void useHardwareSerial(HardwareSerial serial);
		void useSoftwareSerial(SoftwareSerial serial);
		void setAssigner(char assigner_);
		void setVerbose(bool flag=true);
		void handle();
		void addInt(String name, int value=0);
		void setInt(String name, int value);
		int getInt(String name);
};

extern class Serial_Variables SerialVariables;

#endif
