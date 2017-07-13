#pragma once

enum class pins { encoderA = 20, encoderB = 52, knife = 50, forRev1 = 48, forRev2 = 46,
	handDrive1 = 44, handDrive2 = 42, emergency = 40, handAuto = 38, 
	gearRev = 22, gearForv = 24, gearSpeed = 23, sound = 25};

class ControlPins
{
public:
	ControlPins();
	void SetPin(int num, byte value);
	static bool ReadPin(int num) 
	{
		return bool(digitalRead(num));
	};
	void Restart(bool forv, bool speed);
	void RunGear(bool forv, bool speedUp);
	void StopGear();
	bool* ScanPins();
	void UpdateInputs();
	void CheckForStop();
	void CheckButtons();
	~ControlPins();
private:

	int MapFunc(String name);
	//bool encoderA = false;
	bool encoderB = false;
	bool knife = false;
	bool forRev1 = false;
	bool forRev2 = false;
	bool handDrive1 = false;
	bool handDrive2 = false;
	bool emergency = false;
	bool handAuto = false;
	bool gearForv = false;
	bool gearSpeed = false;
	bool sound = false;

	bool knifeSwitch = false;
	//long encoderCounter = 0; // mm counter
};

