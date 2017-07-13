#include "ControlPins.h"


ControlPins::ControlPins()
{
	pinMode((int)pins::encoderB, INPUT);
	pinMode((int)pins::forRev1, INPUT);
	pinMode((int)pins::forRev2, INPUT);
	pinMode((int)pins::handDrive1, INPUT);
	pinMode((int)pins::handDrive2, INPUT);
	pinMode((int)pins::emergency, INPUT);
	pinMode((int)pins::knife, INPUT);
	pinMode((int)pins::handAuto, INPUT);
	pinMode((int)pins::gearForv, OUTPUT);
	pinMode((int)pins::gearRev, OUTPUT);
	pinMode((int)pins::gearSpeed, OUTPUT);
	pinMode((int)pins::sound, OUTPUT);
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

ControlPins::~ControlPins()
{
}
