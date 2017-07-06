#include <LiquidCrystal.h>

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