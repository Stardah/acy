#pragma once
#include <Arduino.h>

enum class pins { encoderA = 20, encoderB = 21, knife = 50, forRev1 = 48, forRev2 = 46,
	handDrive1 = 44, handDrive2 = 42, emergency = 40, handAuto = 38, 
	gearRev = 22, gearForv = 24, gearSpeed = 23, sound = 25};

class ControlPins
{
public:
	ControlPins(int error);
	static bool ReadPin(int num) 
	{
		return bool(digitalRead(num));
	};
	void Reset();
	void Start(long newlength, int newparts, int encoderCounter);
	void Stop();
	bool* ScanPins();
	int UpdateInputs(int encoderCounter);

	void SetEpsCool(int eps_, int coolDown_)
	{
		eps = eps_;
		coolDown = coolDown_;
	};
	int GetLength() 
	{
		//if (knifeSwitch) return length;
		return encoderCounterRef-encoderLength;
	};
	int GetParts()
	{
		return encoderParts;
	};
	~ControlPins();
private:
	void RunGear();
	void StopGear();
	void HandMode(int encoderCounter);
	int AutoMod(int encoderCounter);
	void Sound(int time);
	// Hand 
	bool forRev1 = false;
	bool forRev2 = false;
	bool handDrive1 = false;
	bool handDrive2 = false;
	// All
	bool emergency = false;
	// Auto
	bool ifAuto = false;		
	bool gearForv = true;	// out
	bool gearSpeed = true;	// out
	//bool sound = false;		// out
	bool knife = false;

	bool knifeSwitch = false;
	bool runOn = false;
	bool rollback = false;
	bool firstIteration = true;
	//long encoderCounter = 0; // mm counter
	int encoderLength = 0;
	int encoderParts = 0;
	int encoderCounterRef;
	int length = 0;
	int parts = 0;
	int eps = 0;
	int coolDown = 0;
	int kostyl = 0;
	int notify = -1;
};

