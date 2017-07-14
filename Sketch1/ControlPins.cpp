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

void ControlPins::Start(long newlength, int newparts, int& encoderCounter)
{
	length = newlength;
	parts = newparts;
	runOn = true;
	encoderLength = encoderCounter; // set init conter to current length
}

void ControlPins::Stop()
{
	length = 0;
	parts = 0;
	runOn = false;
	gearSpeed = true;
}

void ControlPins::RunGear()
{
	if (forRev1 || forRev2) digitalWrite((int)pins::gearForv, HIGH);
	else digitalWrite((int)pins::gearRev, HIGH);
	if (gearSpeed) digitalWrite((int)pins::gearSpeed, HIGH);
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

void ControlPins::UpdateInputs(int& encoderCounter)
{
	knife      = ReadPin((int)pins::knife);
	forRev1    = ReadPin((int)pins::forRev1);
	forRev2    = ReadPin((int)pins::forRev2);
	handDrive1 = ReadPin((int)pins::handDrive1);
	handDrive2 = ReadPin((int)pins::handDrive2);
	emergency  = ReadPin((int)pins::emergency);
	ifAuto     = ReadPin((int)pins::handAuto);

	// TODO: notifications
	if (emergency) StopGear();

	if (runOn) AutoMod(encoderCounter);
	else HandMode();

}

void ControlPins::HandMode()
{
	if (handDrive1 && handDrive2 || forRev1 && forRev2) // if both then stop
		StopGear(); 
	else if ((handDrive1 || handDrive2) && (forRev1 || forRev2)) // if presed move then move 
		RunGear(); 
	else StopGear(); 									// if not move then stop								
}

void ControlPins::AutoMod(int& encoderCounter)
{
	// Knife's up/down motions
	if (!knife)
	{
		knifeSwitch = true;
	}
	if (knife && knifeSwitch)
	{
		knifeSwitch = false;
		encoderParts++; // + 1 part
		if (parts != encoderParts) RunGear();
	}
	// end knife
	if (encoderCounter - encoderLength > length) // It's time to cut
	{
		StopGear();						// stop gear
		knifeSwitch = true;				// wait for cut
		encoderLength = encoderCounter; // update length
	}
	if (parts == encoderParts) Stop(); // If all parts done stop process
}

ControlPins::~ControlPins()
{
}
