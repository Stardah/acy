#include "ControlPins.h"

ControlPins::ControlPins()
{
	pinMode((uint8_t)pins::encoderB, INPUT);
	pinMode((uint8_t)pins::forRev1, INPUT);
	pinMode((uint8_t)pins::forRev2, INPUT);
	pinMode((uint8_t)pins::handDrive1, INPUT);
	pinMode((uint8_t)pins::handDrive2, INPUT);
	pinMode((uint8_t)pins::emergency, INPUT);
	pinMode((uint8_t)pins::knife, INPUT);
	pinMode((uint8_t)pins::handAuto, INPUT);

	pinMode((uint8_t)pins::gearForv, OUTPUT);
	pinMode((uint8_t)pins::gearRev, OUTPUT);
	pinMode((uint8_t)pins::gearSpeed, OUTPUT);
	pinMode((uint8_t)pins::sound, OUTPUT);
}

void ControlPins::Restart(bool forv, bool speed)
{
	knifeSwitch = false;
	gearForv = forv;
	gearSpeed = speed;
}

void ControlPins::RunGear(bool forv, bool speedUp)
{
	if (forv) digitalWrite((int)pins::gearForv, HIGH);
	else digitalWrite((int)pins::gearRev, HIGH);
	if (speedUp) digitalWrite((int)pins::gearSpeed, HIGH);
	else digitalWrite((int)pins::gearSpeed, LOW);
}

void ControlPins::StopGear()
{
	digitalWrite((int)pins::gearForv, LOW);
	digitalWrite((int)pins::gearRev, LOW);
	digitalWrite((int)pins::gearSpeed, LOW);
}

///
///Returns array of bool states of pins
///
bool* ControlPins::ScanPins()
{
	bool scan[5];
	for (int i = 0; i < 5; i++) scan[i] = true;
	return  scan;
}

void ControlPins::UpdateInputs()
{
	encoderB   = ReadPin((int)pins::encoderB);
	knife      = ReadPin((int)pins::knife);
	forRev1    = ReadPin((int)pins::forRev1);
	forRev2    = ReadPin((int)pins::forRev2);
	handDrive1 = ReadPin((int)pins::handDrive1);
	handDrive2 = ReadPin((int)pins::handDrive2);
	emergency  = ReadPin((int)pins::emergency);
	handAuto   = ReadPin((int)pins::handAuto);

	// TODO: notifications
	if (emergency) StopGear();
	// Knife's up/fown mitions
	if (!knife)
	{
		StopGear();
		knifeSwitch = true;
	}
	if (knife && knifeSwitch)
	{
		knifeSwitch = false;
		RunGear(gearForv, gearSpeed);
	}
	// end knife
}

///
///Returns the number of pin by device name
///
int ControlPins::MapFunc(String name)
{
	if (name == "encoderA") return 0;
	if (name == "encoderB") return 0;
	if (name == "knife") return 0;
	if (name == "casing") return 0;
	if (name == "btnauto") return 0;
	if (name == "engineRun") return 0;
	if (name == "engineSide") return 0;
	if (name == "engineSpeed") return 0;
		return -1;
}

///
///Sets the state of pin by device name and pin value
///
void ControlPins::SetPin(int num, byte value)
{
	if (value > 1) value = 1;
	//int num = MapFunc(name);
	digitalWrite(num, value);
}



ControlPins::~ControlPins()
{
}
