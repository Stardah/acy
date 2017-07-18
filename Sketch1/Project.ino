#include <Key.h>
#include <Keypad.h>
#include "Menu.h" 
#include "ControlPins.h"
#include "ProgStruct.cpp"
#include <LiquidCrystal.h>
#include <EEPROM.h>

// Display
LiquidCrystal lcd(19, 18, 17, 16, 15, 14); 

// Keypad
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] =
{ // Array buttons-chars
	{ '1','2','3','A'},
	{ '4','5','6','B'},
	{ '7','8','9','C'},
	{ '*','0','#','D'}
};
byte rowPins[ROWS] = { 6, 7, 8, 9 }; // Connect to the row pinouts of the keypad
byte colPins[COLS] = { 2, 3, 4, 5 }; // Connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// End keypad

// External classes
Menu menu(lcd);	// Menu update
ControlPins controlPins(20); // Pins state update, error = 20

PROG programs; // Actually don't need this
Menus cash = Menus::Inp; // Save previous menu
bool progRun = false; // Access to write program
bool serviceOn = false; // Service mode on
long notification = -1;
long notificationWas = -1;
bool notifyAwait = false;
volatile long encoderCounter = 0; // Encoder mm counter

// Settings
long eps = 11;
long epsOld = 11;
long coolDown = 12;
long coolDownOld = 12;
long nozh = 5;
long nozhOld = 5;
//volatile bool encoderB = false;
long kostyl = 0; // TODO replace it with timer

void Addprog(long leng, long amt)
{
	programs.leng = leng;
	programs.amt = amt;
	menu.UpdateValues(leng, amt, nozh);
}

// addr		var
//	0		eps < 100
//  1       coolDown/256 
//  2       coolDown%256
void setup()
{
	//read settings
	eps = EEPROM.read(0);
	coolDown = EEPROM.read(1) * 256 + EEPROM.read(2);
	nozh = EEPROM.read(3);

	epsOld = eps;
	coolDownOld = coolDown;
	nozhOld = nozh;

	controlPins.SetEpsCool(eps, coolDown, nozh);

	//setup
	lcd.begin(16, 2);

	Addprog(200, 4); // ??? recover previous prog
	menu.DrawMenu();
	lcd.cursor();
	lcd.blink();

	attachInterrupt(3, EncoderChange, FALLING);
}

void loop() 
{
	char key = keypad.getKey();					// Update Input
	notification = controlPins.UpdateInputs(encoderCounter);	// Update Gear
	notifyAwait = notification != -1;

	kostyl++;
	if (kostyl > 2000)
	{
		if (notifyAwait)
		{
			NotifyMod();
			menu.Notification(notification);
		}
		else // From AHTUNG to smth
			if (notificationWas == 0 && notification != 0)
			{
				lcd.clear();
				menu.DrawMenu();
			}
		else
		if (progRun)
		{
			menu.RTUpdate(controlPins.GetLength(), controlPins.GetParts()); // Display current values
			menu.DrawMenu();
			kostyl = 0;
			if (controlPins.GetParts() == programs.amt)
			{
				menu.SetMenuMode(Menus::Inp);
				progRun = false;
				lcd.cursor();
				lcd.blink();
				menu.DrawMenu();
				controlPins.Reset();
			}
		}
		else
		if (serviceOn) 
			menu.DrawService(PinsUpdate(), encoderCounter);

		kostyl = 0;

		notificationWas = notification;
	}

	if (key != NO_KEY && !notifyAwait)
	{
		if (serviceOn) ServiceMode(key);
		else if (progRun) RunningMode(key);
		else MenuMode(key);
		menu.DrawMenu();// Update Menu
	}
}

void UpdateSettings() 
{
	if (eps != epsOld) 
	{
		EEPROM.write(0, eps);
		epsOld = eps;
	}
	if (coolDown != coolDownOld)
	{
		EEPROM.write(1, highByte(coolDown));
		EEPROM.write(2, lowByte(coolDown));
		coolDownOld = coolDown;
	}
	if (nozh != nozhOld)
	{
		EEPROM.write(3, nozh);
		nozhOld = nozh;
	}
}

long inputs[12];
long* PinsUpdate() 
{
	inputs[0] = digitalRead((long)pins::forRev1);
	inputs[1] = digitalRead((long)pins::forRev2);
	inputs[2] = digitalRead((long)pins::handDrive1);
	inputs[3] = digitalRead((long)pins::handDrive2);
	inputs[4] = digitalRead((long)pins::emergency);
	inputs[5] = digitalRead((long)pins::handAuto);
	inputs[6] = digitalRead((long)pins::knife);
	inputs[7] = false;//digitalRead((long)pins::gearForv);
	inputs[8] = false;//digitalRead((long)pins::gearSpeed);
	inputs[9] = false;//digitalRead((long)pins::sound);
	inputs[10] =digitalRead((long)pins::encoderA);
	inputs[11] =digitalRead((long)pins::encoderB);
	return inputs;
}

void ServiceMode(char key)
{
	switch (key)
	{
	case '*':
		menu.DelLast();
		break;
	case 'B':
		menu.Up();
		break;
	case 'C':
		menu.Down();
		break;
	case 'A':
		break;
	case '#':
		break;
	case 'D':
		serviceOn = false;
		menu.ApplySettings(eps, coolDown, nozh); // Get input
		UpdateSettings();
		controlPins.SetEpsCool(eps,coolDown, nozh); // Update eps and coolDown in controlPins
		if (cash == Menus::Run) menu.RunProg(programs.leng, programs.amt);
		else menu.SetMenuMode(cash);
		menu.DrawMenu();
		break;
	default:
		menu.Input(key);
		break;
	}
}

void NotifyMod() 
{
	if (notificationWas != 0 && notification == 0)
	{
		lcd.cursor();
		lcd.blink();
		if (controlPins.GetAuto() && progRun) controlPins.Stop();
		progRun = false;
		controlPins.Reset();
		menu.SetMenuMode(Menus::Inp);
	}
}

void RunningMode(char key)
{
	if (!controlPins.GetAuto())
	if (key == 'A') {
		lcd.cursor();
		lcd.blink();
		progRun = false;
		controlPins.Stop();
		controlPins.Reset();
		menu.SetMenuMode(Menus::Inp);
	}
}

void MenuMode(char key) 
{
	switch (key)
	{
	case '*':
		menu.DelLast();
		break;
	case '#':
		break;
	case 'B':
		menu.Up();
		break;
	case 'C':
		menu.Down();
		break;
	case 'A':
		encoderCounter = 0;
		menu.ApplyInput(programs.leng, programs.amt);
		lcd.noCursor();
		lcd.noBlink();
		controlPins.Reset();
		controlPins.Start(programs.leng, programs.amt, encoderCounter);
		progRun = true;
		menu.RunProg(programs.leng, programs.amt);
		break;
	case 'D': // Service
		serviceOn = true;
		cash = menu.getMenu();
		menu.SetMenuMode(Menus::Service);
		menu.UpdateValues(eps, coolDown, nozh); // Service
		break;
	default:
		menu.Input(key);
		break;
	}
}

void EncoderChange() // Interruption
{
	if (digitalRead(21) != 0) // Read encoderB
		encoderCounter++;
	else
		encoderCounter--;
};