#include <LiquidCrystal.h>
#include <Key.h>
#include <Keypad.h>



// Class for menu
class Menu
{
public:
	Menu() {}
	Menu(LiquidCrystal lcd);
	~Menu();

	void PrintWord(LiquidCrystal lcd, char word, int x, int y);

	void PrintWord(LiquidCrystal lcd, String word, int x, int y);

	void DrawMenu(LiquidCrystal lcd);

	void Notification(LiquidCrystal lcd, int i);

	void Input(char word);

	void DelLast(LiquidCrystal lcd);

	void Right(LiquidCrystal lcd);

	void Left(LiquidCrystal lcd);
	
	void Up(LiquidCrystal lcd);
	
	void Down(LiquidCrystal lcd);

	int maxX = 3, maxY = 4;
	String objects[3][4];

	int curX = 0;
	int curY = 0;
};


Menu::Menu(LiquidCrystal lcd)
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (j!=3) objects[i][j] = "(              ";
			else objects[i][j] = "|______________|";
		}
	}
	objects[0][0] = "Programms:";
	objects[0][1] = "1:123_232";
	objects[0][2] = "2:\(@_@)/";
	objects[1][0] = "3: empty";
	objects[2][0] = "World";
	// Print a message to the LCD.
	lcd.print("Input: ");
}

Menu::~Menu()
{
}

void Menu::DrawMenu(LiquidCrystal lcd)
{

	lcd.setCursor(0, 1);
	lcd.print(objects[curX][curY+1]);
	lcd.setCursor(0, 0);
	lcd.print(objects[curX][curY]);
}

void Menu::Notification(LiquidCrystal lcd, int i)
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(notification[i]);
}

void Menu::Input(char ch) 
{
	if (curY != 0 && objects[curX][curY].length() < 20)
	{
		objects[curX][curY] += ch;
	}
}

void Menu::DelLast(LiquidCrystal lcd)
{
	if (curY != 0 && objects[curX][curY].length() > 2) 
	{
		objects[curX][curY] = objects[curX][curY].substring(0, objects[curX][curY].length() - 1);
		lcd.clear();
	}
}

void Menu::PrintWord(LiquidCrystal lcd, char word, int x, int y)
{
	lcd.setCursor(x, y);
	lcd.print(word);
}

void Menu::PrintWord(LiquidCrystal lcd, String word, int x, int y)
{
	lcd.setCursor(x, y);
	lcd.print(word);
}

void Menu::Right(LiquidCrystal lcd)
{
	lcd.scrollDisplayLeft();
}

void Menu::Left(LiquidCrystal lcd)
{
	lcd.scrollDisplayRight();
}

void Menu::Down(LiquidCrystal lcd)
{
	lcd.clear();
	if (curY < maxY-2) curY++;
}

void Menu::Up(LiquidCrystal lcd)
{
	lcd.clear();
	if (curY > 0) curY--;
}

//###################################################################
//###################################################################
//###################################################################
//###################################################################
//###################################################################
//###################################################################
//###################################################################

LiquidCrystal lcd(19, 18, 17, 16, 15, 14); // Display
const String notification[] = { "УП записана","Ожидание резки","УП выполнена", "Ошибка" };

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