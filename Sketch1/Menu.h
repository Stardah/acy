#include <LiquidCrystal.h>

enum Menus { Main, Inp, Stop, Run, MenuInp };

class Menu
{
public:
	Menu(const LiquidCrystal& lcdInit);
	~Menu() = default;

	void UpdateProgRaw(int id, int leng, int amt);
	void SetMenuMode(int newMenu);
	void ApplyInput(int id, int& leng, int& amt);
	void DrawMenu();
	void RunProg(int id, int leng, int amt); // Running menu switch
	void Notification(int i);
	void Input(char word);
	void InputMenu(char word);
	void DelLast();
	void Right();
	void Left();
	void Up();
	void Down();
	void Toast(String str);
	int getX() const;
	int getY() const;

private:
	static const size_t maxX = 5;
	static const size_t maxY = 16;

	LiquidCrystal lcd;
	String items[maxX][maxY];
	int curX = 0;
	int curY = 0;
};