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
	gearSpeed = true;
	gearForv = true;
	length = 0;
	parts = 0;
	encoderLength = 0;
	encoderParts = 0;
	firstIteration = true;
	runOn = false;
	notify = -1;
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
	if (ifAuto) Sound(700);
	//RunGear();
}


///
/// Just stop the process, calls when has cut all parts
///
void ControlPins::Stop()
{
	//length = 0;
	//parts = 0;
	runOn = false;
	notify = -1;
	StopGear();
	if (ifAuto)
	{
		Sound(300);
		delay(300);
		Sound(300);
		delay(300);
		Sound(300);
	}
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
}

///
/// Stops gear
///
void ControlPins::StopGear()
{
	digitalWrite((int)pins::gearForv, LOW);
	digitalWrite((int)pins::gearRev, LOW);
	digitalWrite((int)pins::gearSpeed, LOW);
    digitalWrite((int)pins::sound, LOW);
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
// -1 OK
//  0 FULL_STOP
//  1 KNIFE
//  2
int ControlPins::UpdateInputs(int encoderCounter)
{
	knife = !ReadPin((int)pins::knife);
	forRev1 = ReadPin((int)pins::forRev1);
	forRev2 = ReadPin((int)pins::forRev2);
	handDrive1 = !ReadPin((int)pins::handDrive1);
	handDrive2 = !ReadPin((int)pins::handDrive2);
	emergency = ReadPin((int)pins::emergency);
	ifAuto = !ReadPin((int)pins::handAuto);

	encoderCounterRef = encoderCounter;

	// TODO: notifications
	if (emergency)
	{
		StopGear();
		return 0; // FULL_STOP
	}
	if (!ifAuto) HandMode(encoderCounter);	// Hand Mode
	else if (runOn && ifAuto)				// Auto Mode 
	{
		if (firstIteration)// If it is the first iteration of program
		{
			knifeSwitch = true; // Wait for knife cut
			notify = 1;
			encoderLength = encoderCounter;	// Update current encoderLength
		}
		AutoMod(encoderCounter); // Do auto stuff
	}
	return notify;
}

///
/// When emploee controls the process himself
///
void ControlPins::HandMode(int encoderCounter)
{
	firstIteration = true;
	if (knife) encoderLength = encoderCounter;
	if (handDrive1 && handDrive2) // if both then stop
		StopGear(); 
	else if (handDrive1 || handDrive2) // if presed move then move 
		RunGear(); 
	else StopGear(); // if do not move then stop		
	notify = -1;
}

///
/// When controller controls the process
///
int ControlPins::AutoMod(int encoderCounter)
{
	if (rollback) // Rollback (2nd step)
	{
		if (encoderCounter - encoderLength <= length + eps) // We've got it
		{
			StopGear();
			gearForv = true;	// Move Forward
			gearSpeed = true;	// Whith high speed
			rollback = false;	// End rollback
			Sound(300);
			knifeSwitch = true;	// Waiting for a cut
			notify = 1; // KNIFE (notiry about cut movement)
		}
		else RunGear();	// If we stil need rollback, just continue
	}
	else // Move forward
	{
		if (knife && knifeSwitch) // Waiting for a cut. Knife's up-down motion.
		{
			knifeSwitch = false;	// Stop waiting for a cut
			if (firstIteration) firstIteration = false; // First iter. already started, so make if false
			else encoderParts++;			// + 1 part (increment)
			encoderLength = encoderCounter;	// Update current encoder length
		}
		// end knife
		if (!knifeSwitch) // Run (1st step)
		{
			if (!knife)
			{
				notify = -1;
				RunGear(); // Run if knife is up

				if (encoderCounter - encoderLength >= length - eps) // It's time to cut but...
				{
					StopGear();			// Stop engine
					delay(coolDown);	// Wait while drive is stoping
					rollback = true;	// Start Rollback
					gearForv = false;	// and turn gear to reverse
					gearSpeed = false;	// and activate the slowest speed
				}
			}
			else if (encoderCounter - encoderLength > eps)
			{					// if knife is down    
				StopGear();		// Stop gear
				knifeSwitch = true; // Wait for a cut again
									// TODO: may be we should notify about it
			}
		}
		if (parts == encoderParts) Stop(); // If all parts done stop process
	}
}

void ControlPins::Sound(int time)
{
	digitalWrite((int)pins::sound, HIGH);
	delay(time);
	digitalWrite((int)pins::sound, LOW);
}

ControlPins::~ControlPins()
{
}
