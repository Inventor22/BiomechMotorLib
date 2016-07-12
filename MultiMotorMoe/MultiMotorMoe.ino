
#include "L298MotorDriver.h"

const int tolerance = 3;

//speedPin, in13, in24, potPin, tolerance, (bool) printStatements

// TODO: find limits of potentiometers. Placeholders are there now.

L298MotorDriver motor1('1', 2, 4, 5, 1, 0, 234, tolerance);
L298MotorDriver motor2('2', 6, 8, 7, 4, 0, 234, tolerance);
L298MotorDriver motor3('3', 9, 10, 11, 3, 0, 234, tolerance);

void setup() {
	Serial.begin(115200);

	Serial.println("Mutli-Motor driver testing V1.0");

	delay(1000);
}

bool m1updated = false, m2updated = false, m3updated = false;

void loop()
{
	if (m1updated)
	{
		motor1.update();
	} else
	{
		m1updated = motor1.update();
	}
	if (m2updated)
	{
		motor2.update();
	}
	else
	{
		m2updated = motor2.update();
	}
	if (m3updated)
	{
		motor3.update();
	}
	else
	{
		m3updated = motor3.update();
	}


	if (m1updated && m2updated && m3updated)
	{
		Serial.println();
		m1updated = false;
		m2updated = false;
		m3updated = false;
	}
}

void serialEvent() {
	char motor = char(Serial.read());
	delay(10);
	char command = char(Serial.read()); // read command

	Serial.println(motor);
	Serial.println(command);

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

