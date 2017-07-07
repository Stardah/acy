#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>
#include "Menu.h" 
#include "ProgStruct.cpp"

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

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Menu menu(lcd);

PROG programs[16];
int curProg = 0;

void Addprog(int id, int leng, int amt)
{
	programs[id].id = id;
	programs[id].leng = leng;
	programs[id].amt = amt;
	menu.UpdateProgRaw(id, leng, amt);
}

void setup()
{
	lcd.begin(16, 2);
	menu.DrawMenu();
	Serial.begin(9600);

	for (int i = 0; i < 16; i++) {
		programs[i].id = i;
		programs[i].leng = 123;
		programs[i].amt = 22;
	}
	//attachInterrupt(0, IntFun, 2);
}

void loop() 
{
	char key = keypad.getKey();
	if (key != NO_KEY) 
	{
		if (input) InputMode(key);
		else if (progRun) RunningMode(key);
		else if (stop) StopMode(key);
		else MenuMode(key);
		menu.DrawMenu();
	}
}

void RunningMode(char key)
{
	if (key == 'A') {
		progRun = false;
		stop = true;
		menu.StopProg();
	}
}

void StopMode(char key) 
{
	if (stop)
	{
		if (key == '#')
		{
			menu.RunProg(programs[curProg].id, programs[curProg].leng, programs[curProg].amt);
			stop = false;
			progRun = true;
		}
		else if (key == '*')
		{
			menu.MenuMode();
			stop = false;
		}
	}
}

void InputMode(char key)
{
	if (key == '#')
	{
		input = false;
		lcd.noCursor();
		lcd.noBlink();
	}
	else if (key == 'C') menu.DelLast();
	else menu.Input(key);
}

void MenuMode(char key) 
{
	switch (key)
	{
	case '*':
		menu.Left();
		break;
	case '#':
		if (menu.getY() != 0 && !progRun)
		{
			input = true;
			lcd.cursor();
			lcd.blink();
		}
		break;
	case 'B':
		menu.Up();
		break;
	case 'C':
		menu.Down();
		break;
	case 'A':
		if (menu.getY() != 0 && !input)
		{
			progRun = true;
			menu.RunProg(programs[0].id, programs[0].leng, programs[0].amt);
			//lcd.cursor();
			//lcd.blink();
		}
		break;
	default:
		break;
	}
}

void IntFun() 
{
	Serial.println("interrupt");
}