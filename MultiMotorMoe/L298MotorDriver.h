// L298MotorDriver.h

#ifndef _L298MOTORDRIVER_h
#define _L298MOTORDRIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class L298MotorDriver
{
public:
	L298MotorDriver(
		char id, int speedPin, 
		int in13, int in24, 
		int potPin, int tolerance, 
		int potMin, int potMax,
		bool printStatement = true);

	bool update();

	void printStatements(bool flag);

	void parseCommand(char cmd, String& args);

	char getId() const;

private:
	int _enAB, _in13, _in24, _pot;
	
	uint32_t _tNow, _tPrev;
	int _stmt;

	int _actualPosCmd, _tolerance;
	float _rawPosCmd;
	int _rawSpeed;
	int _speed;

	int _potMin, _potMax;

	bool _printStatements;

	const char _id;

	static const uint32_t _updatePeriod = 1000;
	static const int _maxTolerance = 10;

	static const int _motMinPot = 0;
	static const int _motMaxPot = 254;
};

#endif

