#include "ControlPins.h"

ControlPins::ControlPins(int error_)
{
	eps = error_;
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

///
/// Clear variables, casts after stop()
///
void ControlPins::Reset()
{
	knifeSwitch = false;
	rollback = false;
	length = 0;
	parts = 0;
	encoderLength = 0;
	encoderParts = 0;
}

///
/// Initialize variables
///
void ControlPins::Start(long newlength, int newparts, int encoderCounter)
{
	length = newlength;
	parts = newparts;
	runOn = true;
	encoderLength = encoderCounter; // set init conter to current length
	RunGear();
}


///
/// Just stop the process, calls when has cut all parts
///
void ControlPins::Stop()
{
	//length = 0;
	//parts = 0;
	sound = false;
	runOn = false;
	gearSpeed = true;
	gearForv = true;
}

///
/// Runing gear whith current setups for vector and speed
///
void ControlPins::RunGear()
{
	if (!ifAuto)
	{
		if (handDrive1)
		{
			if (forRev1) digitalWrite((int)pins::gearForv, HIGH);
			else digitalWrite((int)pins::gearRev, HIGH);
		}
		if (handDrive2)
		{
			if (forRev2) digitalWrite((int)pins::gearForv, HIGH);
			else digitalWrite((int)pins::gearRev, HIGH);
		}
	}
	else if (gearForv)
	{
		digitalWrite((int)pins::gearForv, HIGH); // Auto forward
		digitalWrite((int)pins::gearRev, LOW);
	}
	else
	{
		digitalWrite((int)pins::gearRev, HIGH); // Auto reverse
		digitalWrite((int)pins::gearForv, LOW);
	}
	if (gearSpeed) digitalWrite((int)pins::gearSpeed, HIGH);
	else digitalWrite((int)pins::gearSpeed, LOW);

	if (sound) digitalWrite((int)pins::sound, HIGH);
	else digitalWrite((int)pins::sound, LOW);
}

///
/// Stops gear
///
void ControlPins::StopGear()
{
	digitalWrite((int)pins::gearForv, LOW);
	digitalWrite((int)pins::gearRev, LOW);
	digitalWrite((int)pins::gearSpeed, LOW);
	if (sound) digitalWrite((int)pins::sound, HIGH);
	else digitalWrite((int)pins::sound, LOW);
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

///
/// update states for all pins and do doings
///
bool wasAuto = false;
void ControlPins::UpdateInputs(int encoderCounter)
{
	knife      = !ReadPin((int)pins::knife);
	forRev1    = ReadPin((int)pins::forRev1);
	forRev2    = ReadPin((int)pins::forRev2);
	handDrive1 = !ReadPin((int)pins::handDrive1);
	handDrive2 = !ReadPin((int)pins::handDrive2);
	emergency  = ReadPin((int)pins::emergency);
	ifAuto     = !ReadPin((int)pins::handAuto);


	encoderCounterRef = encoderCounter;

	// TODO: notifications
	if (emergency) 
	{
		sound = false;
		StopGear();
	}


	if (runOn && ifAuto) 
	{
		if (!wasAuto)// If we just switched from the hand mode
		{
			encoderLength = encoderCounter;
			if (!knife) RunGear();
			// else ALERT!
		}
		AutoMod(encoderCounter);
	}
	else if (!ifAuto) HandMode(encoderCounter);

	wasAuto = ifAuto;
}

///
/// When emploee controls the process himself
///
void ControlPins::HandMode(int encoderCounter)
{
	if (knife) encoderLength = encoderCounter;
	if (handDrive1 && handDrive2) // if both then stop
		StopGear(); 
	else if (handDrive1 || handDrive2) // if presed move then move 
		RunGear(); 
	else StopGear(); 									// if not move then stop								
}

///
/// When controller controls the process
///
void ControlPins::AutoMod(int encoderCounter)
{
	if (rollback) // Rollback
	{
		if (encoderCounter - encoderLength <= length + eps) // We've got it
		{
			StopGear();
			gearForv = true;
			gearSpeed = true;
			rollback = false;
			sound = true;					// sound
			knifeSwitch = true;				// wait for a cut
		}
		else RunGear();
	}
	else
	{
		// Knife's up/down motions
		if (knife && knifeSwitch)
		{
			knifeSwitch = false;
			encoderParts++; // + 1 part
			sound = false;
			if (parts != encoderParts) RunGear();
			encoderLength = encoderCounter;
		}
		// end knife
		if (!knifeSwitch)
			if (encoderCounter - encoderLength >= length) // It's time to cut but...
			{
				StopGear(); // Stop engine
				delay(coolDown);	// CoolDown
				rollback = true;								// Ok then. Rollback a bit
				gearForv = false;								// Turn gear to reverse
				gearSpeed = false;								// Activate the slowest speed
			}
		if (parts == encoderParts) Stop(); // If all parts done stop process
	}
	if (knife && ((encoderCounter - encoderLength)>10)) StopGear(); // Knife down when we run
}

ControlPins::~ControlPins()
{
}
