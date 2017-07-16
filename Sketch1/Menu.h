#include <LiquidCrystal.h>

enum Menus {Inp, Stop, Run, Service, Notify };

class Menu
{
public:
	Menu(const LiquidCrystal& lcdInit);
	~Menu() = default;

	void UpdateValues(int leng, int amt);
	void SetMenuMode(int newMenu);
	void ApplyInput(int& leng, int& amt);
	void DrawMenu();
	void DrawService(int inputs[12], int encoderCounter);
	void RTUpdate(int length, int parts); // To display current length and parts
	void RunProg(int leng, int amt); // Running menu switch
	void Notification(int i);
	void Input(char word);
	void DelLast();
	void Up();
	void Down();
	Menus getMenu() 
	{
		return menuMode;
	}
private:
	static const size_t maxX = 5;
	static const size_t maxY = 8;
	Menus menuMode = Menus::Inp;
	LiquidCrystal lcd;
	String items[maxX][maxY];
	int curX = 0;
	int curY = 0;
	bool upside = true;
	String notification[4] = { "FATAL ERROR","KNIFE"," ","УП выполнена"};
	int length = 0;
	int parts = 0;
};