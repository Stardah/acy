#include "Menu.h"

const String notification[] = { "УП записана","Ожидание резки","УП выполнена", "Ошибка" };
String dlin = "";
String kol = "Ko";
String iz = "";

Menu::Menu(const LiquidCrystal& lcdInit) :
	lcd(lcdInit)
{
	/************************************
	* Это что за магические цифры? 3 и 4
	************************************/
	for (size_t i = 0; i < maxX; i++)
	{
		for (size_t j = 0; j < maxY; j++)
		{
			if (j != maxY - 1)
				items[i][j] = "(              ";
			else 
				items[i][j] = "|______________|";
		}
	}

	/*String s = "";
	for (int ib = 0; ib < 256; ib++) 
	{
		s += char(ib);
		if (ib % 16 == 0) 
		{
			objects[0][ib/16] = s;
			s = String(ib)+":";
		}
	}*/

	dlin += char(224); // d
	dlin += char(187);// l
	// dlin += char(184); // i
	// dlin += char(189); // n
	kol += char(187);
	iz += char(165);
	iz += char(183);
	items[0][0] = "Programms:";
	items[0][1] = "1." + dlin + ":1234 " + kol + ":22";
	items[0][2] = "2." + dlin + ":1234 " + kol + ":22";
	items[2][0] = "Continue: #";
	items[2][1] = "Menu: *";
	
	// Print a message to the LCD.
	lcd.print("Input: ");
}

void Menu::UpdateProgRaw(int id, int leng, int amt)
{
	items[0][id + 1] = String(id) + "." + dlin + ":" + String(leng) + " " + kol + ":" + String(amt);
}

void Menu::DrawMenu()
{
	lcd.setCursor(0, 1);
	lcd.print(items[curX][curY + 1]);
	lcd.setCursor(0, 0);
	lcd.print(items[curX][curY]);
}

void Menu::RunProg(int id, int leng, int amt) 
{
	lcd.clear();
	items[1][0] = String(id) + "." + dlin + ":" + "0" + " " + iz + " " + String(leng);
	items[1][1] = kol + ":" + "0" + " " + iz + " " + String(amt);
	curX = 1;
	curY = 0;
}

void Menu::StopProg()
{
	lcd.clear();
	curX = 2;
	curY = 0;
}

void Menu::MenuMode() 
{
	curX = 0;
	curY = 0;
}

void Menu::Notification(int i)
{
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(notification[i]);
}

void Menu::Input(char ch)
{
	if (curY != 0 && items[curX][curY].length() < 20)
	{
		items[curX][curY] += ch;
	}
}

void Menu::DelLast()
{
	if (curY != 0 && items[curX][curY].length() > 2)
	{
		items[curX][curY] = items[curX][curY].substring(0, items[curX][curY].length() - 1);
		lcd.clear();
	}
}

void Menu::PrintWord(char word, int x, int y)
{
	lcd.setCursor(x, y);
	lcd.print(word);
}

void Menu::PrintWord(String word, int x, int y)
{
	lcd.setCursor(x, y);
	lcd.print(word);
}

void Menu::Right()
{
	lcd.scrollDisplayLeft();
}

void Menu::Left()
{
	lcd.scrollDisplayRight();
}

void Menu::Down()
{
	lcd.clear();
	if (curY < maxY - 2)
		++curY;
}

int Menu::getX() const
{
	return curX;
}

int Menu::getY() const
{
	return curY;
}

void Menu::Up()
{
	lcd.clear();
	if (curY > 0)
		--curY;
}