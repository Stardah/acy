#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>
#include "Menu.h"

LiquidCrystal lcd(19, 18, 17, 16, 15, 14); // Display

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = { // Array buttons-chars
	{ '1','2','3','A'},
	{ '4','5','6','B'},
	{ '7','8','9','C'},
	{ '*','0','#','D'}
};
byte rowPins[ROWS] = { 6, 7, 8, 9 }; // Connect to the row pinouts of the keypad
byte colPins[COLS] = { 2, 3, 4, 5 }; // Connect to the column pinouts of the keypad

bool input = false;		// Menu or input
bool inputLock = false; // Access to write program

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Menu menu(lcd);

void setup()
{
	lcd.begin(16, 2);
	menu.DrawMenu(lcd);
	Serial.begin(9600);
	//attachInterrupt(0, IntFun, 2);
}

void loop() 
{
	char key = keypad.getKey();
	if (key != NO_KEY) 
	{
		if (input)
		{
			if (key == 'A') 
			{
				input = false;
				lcd.noCursor();
				lcd.noBlink();
			}
			else if (key == 'C') menu.DelLast(lcd);
			else menu.Input(key);
		}
		else
		switch (key)
		{
		case '*':
			menu.Left(lcd);
			break;
		 case '#':
			 menu.Right(lcd);
			 break;
		 case 'B':
			 menu.Up(lcd);
			 break;
		 case 'C':
			 menu.Down(lcd);
			 break;
		 case 'A':
			 if (menu.curY != 0 && !inputLock) 
			 {
				 input = true;
				 lcd.cursor();
				 lcd.blink();
			 }
			 break;
		default:
			break;
		}
		menu.DrawMenu(lcd);
	}
}

void IntFun() 
{
	Serial.println("interrupt");
}