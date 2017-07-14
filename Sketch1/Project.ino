#include <Key.h>
#include <Keypad.h>
#include "Menu.h" 
#include "ControlPins.h"
#include "ProgStruct.cpp"
#include <LiquidCrystal.h>

LiquidCrystal lcd(19, 18, 17, 16, 15, 14); // Display

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

bool input = false;		// Menu or input
bool progRun = false; // Access to write program
bool stop = false; // stop menu
int encoderCounter = 0; // mm counter

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Menu menu(lcd);
ControlPins controlPins;

PROG programs;
int curProg = 0; // Current program id

void Addprog(int leng, int amt)
{
	programs.leng = leng;
	programs.amt = amt;
	menu.UpdateProgRaw(leng, amt);
}

void setup()
{
	lcd.begin(16, 2);
	Serial.begin(9600);

	Addprog(0, 0); // recover previous prog

	menu.DrawMenu();
	lcd.cursor();
	lcd.blink();

	attachInterrupt((uint8_t)pins::encoderA, EncoderChange, FALLING);
}

void loop() 
{
	controlPins.UpdateInputs(encoderCounter);	// Update Gear
	char key = keypad.getKey();					// Update Input
	if (key != NO_KEY) 
	{
		if (progRun) RunningMode(key);
		else if (stop) StopMode(key);
		else MenuMode(key);
		menu.DrawMenu();						// Update Menu
	}
}

void RunningMode(char key)
{
	if (key == 'A') {
		progRun = false;
		stop = true;
		menu.SetMenuMode(Menus::Stop);
	}
	menu.RTUpdate(controlPins.GetLength(), controlPins.GetParts()); // Display current values
}

void StopMode(char key) 
{
	if (stop)
	{
		if (key == '#')
		{
			menu.RunProg(programs.leng, programs.amt);
			stop = false;
			progRun = true;
		}
		else if (key == '*')
		{
			menu.SetMenuMode(Menus::Inp);;
			lcd.cursor();
			lcd.blink();
			stop = false;
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
	case 'D':
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
			
		progRun = true;
		curProg = 0;
		menu.RunProg(programs.leng, programs.amt);
		break;
	default:
		menu.Input(key);
		break;
	}
}

void EncoderChange()
{
	if (encoderCounter > 10000) encoderCounter = 1000;
	if (encoderCounter < -10000) encoderCounter = -1000;
	if (controlPins.ReadPin((int)pins::encoderB))
	{
		Serial.println("Encoder++");
		encoderCounter++;
	}
	else
	{
		Serial.println("Encoder--");
		encoderCounter--;
	}
};