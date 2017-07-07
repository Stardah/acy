#include <LiquidCrystal.h>

class Menu
{
public:
	Menu() {}
	Menu(LiquidCrystal& lcd);
	~Menu();

	void PrintWord(LiquidCrystal& lcd, char word, int x, int y);

	void PrintWord(LiquidCrystal& lcd, String word, int x, int y);

	void UpdateProgRaw(LiquidCrystal& lcd, int id, int leng, int amt);

	void InputProg(LiquidCrystal& lcd) 
	{

	}

	void DrawMenu(LiquidCrystal& lcd);

	void RunProg(LiquidCrystal& lcd, int id, int leng, int amt); // Running menu switch

	void StopProg(LiquidCrystal& lcd); // Stop menu switch

	void MenuMode(LiquidCrystal& lcd); // Prog menu switch

	void Notification(LiquidCrystal& lcd, int i);

	void Input(char word);

	void DelLast(LiquidCrystal& lcd);

	void Right(LiquidCrystal& lcd);

	void Left(LiquidCrystal& lcd);

	void Up(LiquidCrystal& lcd);

	void Down(LiquidCrystal& lcd);

private:
	int maxX = 3, maxY = 16;
	String objects[3][17];

	int curX = 0;
	int curY = 0;
};