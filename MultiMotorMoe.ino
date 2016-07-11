
#include "L298MotorDriver.h"

//speedPin, in13, in24, potPin, tolerance, (bool) printStatements
L298MotorDriver motor1(3, 4, 5, 5, 3);
L298MotorDriver motor2(6, 7, 8, 4, 3);
L298MotorDriver motor3(9, 10, 11, 3, 3);

void setup() {
	Serial.begin(115200);

	Serial.println("Motor driver testing");

	delay(1000);
}

void loop() {
	motor1.update();
	motor2.update();
	motor3.update();
}

void serialEvent() {
	char motor = char(Serial.read());
	char command = char(Serial.read()); // read command

	String val = "";

	delay(5);
	char currentChar = ' ';
	while (Serial.available()) {
		currentChar = char(Serial.read());

		if (currentChar == '$')
			break;

		val += currentChar;

		delay(5);
	}

	switch (motor)
	{
	case '1':
		motor1.parseCommand(command, val);
		break;
	case '2':
		motor2.parseCommand(command, val);
		break;
	case '3':
		motor3.parseCommand(command, val);
		break;
	default:
		Serial.print("Motor '");
		Serial.print(motor);
		Serial.print("' is not a valid option.");
		break;
	}


}

