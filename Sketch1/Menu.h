#include <LiquidCrystal.h>

class Menu
{
public:
	Menu(const LiquidCrystal& lcdInit);
	~Menu() = default;

	void PrintWord(char word, int x, int y);
	void PrintWord(String word, int x, int y);
	void UpdateProgRaw(int id, int leng, int amt);
	void InputProg() = delete;
	void DrawMenu();
	void RunProg(int id, int leng, int amt); // Running menu switch
	void StopProg(); // Stop menu switch
	void MenuMode(); // Prog menu switch
	void Notification(int i);
	void Input(char word);
	void DelLast();
	void Right();
	void Left();
	void Up();
	void Down();
	int getX() const;
	int getY() const;

private:
	static const int maxX = 3;
	static const int maxY = 16;

	LiquidCrystal lcd;
	String items[maxX][maxY - 1];
	int curX = 0;
	int curY = 0;
};