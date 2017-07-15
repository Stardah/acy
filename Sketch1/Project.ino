#include <Key.h>
#include <Keypad.h>
#include "Menu.h" 
#include "ControlPins.h"
#include "ProgStruct.cpp"
#include <LiquidCrystal.h>

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
bool stop = false; // stop menu
bool serviceOn = false; // Service mode on
volatile int encoderCounter = 0; // Encoder mm counter
//volatile bool encoderB = false;
int kostyl = 0; // TODO replace it with timer

void Addprog(int leng, int amt)
{
	programs.leng = leng;
	programs.amt = amt;
	menu.UpdateProgRaw(leng, amt);
}

void setup()
{
	lcd.begin(16, 2);

	Addprog(200, 4); // recover previous prog

	menu.DrawMenu();
	lcd.cursor();
	lcd.blink();

	attachInterrupt(3, EncoderChange, FALLING);
	//attachInterrupt(2, EncoderChangeB, CHANGE);
}

void loop() 
{
	controlPins.UpdateInputs(encoderCounter);	// Update Gear
	if (progRun)
	{
		kostyl++;
		if (kostyl > 4000)
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
	}
	char key = keypad.getKey();					// Update Input
	if (serviceOn)
	{
		kostyl++;
		if (kostyl > 2000) 
		{
			menu.DrawService(PinsUpdate(), encoderCounter);
			kostyl = 0;
		}
	}
	if (key != NO_KEY) 
	{
		if (key == 'D')  // Service
		{
			serviceOn = !serviceOn;
			if (!serviceOn) 
			{
				if (cash == Menus::Run) menu.RunProg(programs.leng,programs.amt);
				else menu.SetMenuMode(cash);
				menu.DrawMenu();
			}
			else
			{
				cash = menu.getMenu();
				menu.SetMenuMode(Menus::Service);
			}
		}
		else
		{
			if (serviceOn) ServiceMode(key);
			else if (progRun) RunningMode(key);
			else if (stop) StopMode(key);
			else MenuMode(key);
			menu.DrawMenu();						// Update Menu
		}
	}
}

int inputs[12];
int* PinsUpdate() 
{
	inputs[0] = digitalRead((int)pins::forRev1);
	inputs[1] = digitalRead((int)pins::forRev2);
	inputs[2] = digitalRead((int)pins::handDrive1);
	inputs[3] = digitalRead((int)pins::handDrive2);
	inputs[4] = digitalRead((int)pins::emergency);
	inputs[5] = digitalRead((int)pins::handAuto);
	inputs[6] = digitalRead((int)pins::knife);
	inputs[7] = digitalRead((int)pins::gearForv);
	inputs[8] = digitalRead((int)pins::gearSpeed);
	inputs[9] = digitalRead((int)pins::sound);
	inputs[10] =digitalRead((int)pins::encoderA);
	inputs[11] =digitalRead((int)pins::encoderB);
	return inputs;
}

void ServiceMode(char key)
{
	switch (key)
	{
	case 'B':
		menu.Up();
		break;
	case 'C':
		menu.Down();
		break;
	default:
		break;
	}
}

void RunningMode(char key)
{
	if (key == 'A') {
		progRun = false;
		stop = true;
		menu.SetMenuMode(Menus::Stop);
	}
}

void StopMode(char key) 
{
	if (stop)
	{
		if (key == '#') // Продолжаем
		{
			menu.RunProg(controlPins.GetLength(), controlPins.GetParts());
			progRun = true;
			stop = false;
			progRun = true;
		}
		else if (key == '*') // Стоп
		{
			menu.SetMenuMode(Menus::Inp);;
			lcd.cursor();
			lcd.blink();
			stop = false;
			controlPins.Stop();
			controlPins.Reset();
		}
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
		menu.ApplyInput(programs.leng, programs.amt);
		break;
	case 'B':
		menu.Up();
		break;
	case 'C':
		menu.Down();
		break;
	case 'A':
		menu.ApplyInput(programs.leng, programs.amt);
		lcd.noCursor();
		lcd.noBlink();
		controlPins.Start(programs.leng, programs.amt, encoderCounter);
		progRun = true;
		menu.RunProg(programs.leng, programs.amt);
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

void EncoderChangeB() // Interruption
{
	//encoderB = (bool)digitalRead(21);
};