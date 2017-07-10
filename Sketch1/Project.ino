#include <Key.h>
#include <Keypad.h>
#include "Menu.h" 
#include "ProgStruct.cpp"
#include <LiquidCrystal.h>

#define modeMain 0
#define modeInp 1
#define modeStop 2
#define modeRun 3
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
int curProg = 0; // Current program id
long encoderCounter = 0;

int encoderStateWas = 0;
int encoderStateNow = 0;

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
	//Serial.begin(9600);

	for (int i = 0; i < 15; i++) {
		programs[i].id = i;
		programs[i].leng = 123;
		programs[i].amt = 22;
		Addprog(programs[i].id, programs[i].leng, programs[i].amt);
	}
	pinMode(20, INPUT);
	pinMode(52, INPUT);
	menu.DrawMenu();
	attachInterrupt(3, EncoderChange, FALLING);
	//attachInterrupt(1, EncoderDec, CHANGE);
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
		menu.SetMenuMode(Menus::Stop);
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
			menu.SetMenuMode(Menus::Main);;
			stop = false;
		}
	}
}

void InputMode(char key)
{
	if (key == '#')
	{
		input = false;
		menu.ApplyInput(curProg, programs[curProg].leng, programs[curProg].amt);
		lcd.noCursor();
		lcd.noBlink();
		menu.SetMenuMode(Menus::Main, curProg);
	}
	else if (key == '*') menu.DelLast();
	else if (key == 'B') menu.Up();
	else if (key == 'C') menu.Down();
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
		if (!progRun)
		{
			input = true;
			menu.SetMenuMode(Menus::Inp);
			lcd.cursor();
			lcd.blink();
		}
		break;
	case 'D':
		menu.Toast(String(curProg));
		delay(5000);
		break;
	case 'B':
		menu.Up();
		if (curProg>0) curProg--;
		break;
	case 'C':
		menu.Down();
		if (curProg<16) curProg++;
		break;
	case 'A':
		if (menu.getY() != 0 && !input)
		{
			progRun = true;
			curProg = 0;
			menu.RunProg(programs[0].id, programs[0].leng, programs[0].amt);
			//lcd.cursor();
			//lcd.blink();
		}
		break;
	default:
		break;
	}
}

void EncoderChange()
{
	if (encoderCounter > 10000) encoderCounter = 1000;
	if (encoderCounter < -10000) encoderCounter = -1000;
	if (digitalRead(52) == 1)
	{
		Serial.println("Encoder++");
		encoderCounter++;
	}
	else
	{
		Serial.println("Encoder--");
		encoderCounter--;
	}
}