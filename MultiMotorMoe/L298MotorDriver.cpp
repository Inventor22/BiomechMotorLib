// 
// 
// 

#include "L298MotorDriver.h"


L298MotorDriver::L298MotorDriver(
	char id, int speedPin, 
	int in13, int in24, 
	int potPin, int tolerance, 
	int potMin, int potMax,
	bool printStatements)
	: _id(id)
{
	this->_enAB = speedPin;
	this->_in13 = in13;
	this->_in24 = in24;

	this->_pot = potPin;

	this->_tolerance = tolerance;

	this->_printStatements = printStatements;

	this->_rawSpeed = 0;
	this->_rawPosCmd = 100;

	this->_speed = 100;

	this->_stmt = -1;

	this->_potMin = potMin;
	this->_potMax = potMax;

	pinMode(this->_enAB, OUTPUT);
	pinMode(this->_in13, OUTPUT);
	pinMode(this->_in24, OUTPUT);

	analogWrite(this->_enAB, 0);
	digitalWrite(this->_in13, LOW);
	digitalWrite(this->_in24, LOW);
}

bool L298MotorDriver::update()
{
	static bool printed = false;

	_tNow = millis();
	if (_printStatements && (_tNow - _tPrev > _updatePeriod)) {
		_tPrev = _tNow;

		Serial.print("Id: ");
		Serial.print(_id);

		Serial.print(", stmt: ");
		Serial.print(_stmt);

		Serial.print(", actualPosCmd: ");
		Serial.print(_actualPosCmd);

		Serial.print(", Raw Motor Pos: ");
		Serial.println(analogRead(_pot));

		printed = true;
	} else
	{
		printed = false;
	}

	int actualPos = analogRead(_pot);
	if (actualPos < (_actualPosCmd - _tolerance)) {
		digitalWrite(_in13, HIGH);
		digitalWrite(_in24, LOW);
		analogWrite(_enAB, _speed);
		_stmt = 0;
	}
	else if (actualPos >(_actualPosCmd + _tolerance)) {
		digitalWrite(_in13, LOW);
		digitalWrite(_in24, HIGH);
		analogWrite(_enAB, _speed);
		_stmt = 1;
	}
	else {
		digitalWrite(_in13, LOW);
		digitalWrite(_in24, LOW);
		analogWrite(_enAB, 0);
		_stmt = 2;
	}

	return printed;
}

void L298MotorDriver::parseCommand(char cmd, String& args)
{
	switch (cmd) {
	case 'd': // distance
		_rawPosCmd = args.toFloat();

		if (_rawPosCmd < 0 || _rawPosCmd > 100) {
			Serial.println("Position out of range.  Please select between 0 and 100.");
		}
		else {

			_actualPosCmd = map(_rawPosCmd, 0, 100, _motMinPot, _motMaxPot);

			Serial.print("Id: ");
			Serial.print(_id);
			Serial.print(", Received pos command: ");
			Serial.print(_rawPosCmd);
			Serial.print(", actualPosCommand: ");
			Serial.print(_actualPosCmd);
			Serial.print(", pot pos: ");
			Serial.println(analogRead(_pot));
		}
		break;

	case 's': // speed
		_rawSpeed = args.toInt();
		if (_rawSpeed < 0 || _rawSpeed > 100) {
			Serial.print("Id: ");
			Serial.print(_id);
			Serial.println(", Speed out of range.  Choose between 0 and 100.");
			_speed = 0;
		}
		else {
			_speed = map(_rawSpeed, 0, 100, 0, 255);
			Serial.print("Id: ");
			Serial.print(_id);
			Serial.print(", Raw speed: ");
			Serial.print(_rawSpeed);
			Serial.print(", Speed mapped to: ");
			Serial.println(_speed);
		}
		break;

	case 't': // tolerance
		_tolerance = args.toInt();
		if (_tolerance < 0 || _tolerance > _maxTolerance) {
			Serial.print("Id: ");
			Serial.print(_id);
			Serial.println(", tolerance out of range.  Choose between 0 and 10.");
		}
		else {
			Serial.print("Id: ");
			Serial.print(_id);
			Serial.print(", Tolerance set to: ");
			Serial.println(_tolerance);
		}
		break;

	case 'p': // print values
		Serial.print("Id: ");
		Serial.print(_id);
		Serial.print(", position command: ");
		Serial.print(_actualPosCmd);
		Serial.print(", current position: ");
		Serial.print(analogRead(_pot));
		Serial.print(", Speed: ");
		Serial.print(_speed);
		Serial.print(", Tolerance: ");
		Serial.println(_tolerance);
		break;

	default:
		Serial.print("Id: ");
		Serial.print(_id);
		Serial.print(", command: '");
		Serial.print(cmd);
		Serial.println("' not supported.");
		break;
	}
}

void L298MotorDriver::printStatements(bool flag)
{
	_printStatements = flag;
}

char L298MotorDriver::getId() const
{
	return _id;
}
