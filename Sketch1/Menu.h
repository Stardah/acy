#include <LiquidCrystal.h>

enum Menus {Inp, Run, Service, Notify };

class Menu
{
public:
	Menu(const LiquidCrystal& lcdInit);
	~Menu() = default;

	void UpdateValues(long leng, long amt, long width);
	void SetMenuMode(long newMenu);
	void ApplyInput(long& leng, long& amt);
	void ApplySettings(long& leng, long& amt, long& width);
	void DrawMenu();
	void DrawService(long inputs[12], long encoderCounter);
	void RTUpdate(long length, long parts); // To display current length and parts
	void RunProg(long leng, long amt); // Running menu switch
	void Notification(long i);
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
	long curX = 0;
	long curY = 0;
	bool upside = true;
	String notification[4];
	long length = 0;
	long parts = 0;
};