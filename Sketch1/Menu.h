#include <LiquidCrystal.h>

enum Menus { Main, Inp, Stop, Run, Service };

class Menu
{
public:
	Menu(const LiquidCrystal& lcdInit);
	~Menu() = default;

	void UpdateProgRaw(int leng, int amt);
	void SetMenuMode(int newMenu);
	void ApplyInput(int& leng, int& amt);
	void DrawMenu();
	void RTUpdate(int length, int parts); // To display current length and parts
	void RunProg(int leng, int amt); // Running menu switch
	void Notification(int i);
	void Input(char word);
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
	static const size_t maxY = 3;

	LiquidCrystal lcd;
	String items[maxX][maxY];
	int curX = 0;
	int curY = 0;
	bool upside = true;
	//const String notification[] = { "УП записана","Ожидание резки","УП выполнена", "Ошибка" };
	int length = 0;
	int parts = 0;
};