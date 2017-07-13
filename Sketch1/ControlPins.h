#pragma once

#include <assert.h>

class ControlPins
{
public:
	ControlPins(byte pEncoderA, byte pEncoderB, byte pKnife, byte pCasing, byte pAuto, byte pEngineRun, byte pEngineSide, byte pEngineSpeed);
	void SetPin(String name, byte value);
	bool ReadPin(String name);
	bool* ScanPins();
	void UpdateInputs();
	~ControlPins();
private:
	int MapFunc(String name);
	bool encoderA = false;
	bool encoderB = false;
	bool knife = false;
	bool casing = false;
	bool btnAuto = false;
	bool btnHandRun = false;
	bool btnForward = false;
	bool engineSpeedDown = false;
	bool stop = false;
};

///
/// Перечисление для типа девайса
///
enum class PinType
{
	EncoderA,
	EncoderA,
	Knife,
	Casing,
	Auto,
	EngineRun,
	EngineSide,
	EngineSpeed,
	// Каждой ножке можно присвоить значение:
	// Element = 30,
};