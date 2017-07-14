#pragma once
#include <Arduino.h>

enum class pins { encoderA = 20, encoderB = 52, knife = 50, forRev1 = 48, forRev2 = 46,
	handDrive1 = 44, handDrive2 = 42, emergency = 40, handAuto = 38, 
	gearRev = 22, gearForv = 24, gearSpeed = 23, sound = 25};

class ControlPins
{
public:
	ControlPins();
	static bool ReadPin(int num) 
	{
		return bool(digitalRead(num));
	};
	void Restart(bool forv, bool speed);
	void Start(long newlength, int newparts, int& encoderCounter);
	void Stop();
	void RunGear();
	void StopGear();
	bool* ScanPins();
	void UpdateInputs(int& encoderCounter);
	void HandMode();
	void AutoMod(int& encoderCounter);
	int GetLength() 
	{
		return encoderLength;
	};
	int GetParts()
	{
		return encoderParts;
	};
	~ControlPins();
private:
	// Hand 
	bool forRev1 = false;
	bool forRev2 = false;
	bool handDrive1 = false;
	bool handDrive2 = false;
	// All
	bool emergency = false;
	bool encoderA = false; 
	bool encoderB = false;
	// Auto
	bool ifAuto = false;		
	bool gearForv = false;	// out
	bool gearSpeed = true;	// out
	bool sound = false;		// out
	bool knife = false;

	bool knifeSwitch = false;
	bool runOn = false;
	//long encoderCounter = 0; // mm counter
	int encoderCount;
	int encoderLength = 0;
	int encoderParts = 0;
	int length = 0;
	int parts = 0;
};

