#include "Menu.h"

const String notification[] = { "УП записана","Ожидание резки","УП выполнена", "Ошибка" };
String dlin = "";
String kol = "Ko";
String iz = "";

Menu::Menu(LiquidCrystal lcd)
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (j != 3) objects[i][j] = "(              ";
			else objects[i][j] = "|______________|";
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
	}
	*/
	dlin += char(224); // d
	dlin+=char(187);// l
	//dlin += char(184); // i
	//dlin+=char(189); // n
	kol +=char(187);
	iz += char(165);
	iz+=char(183);
	objects[0][0] = "Programms:";
	objects[0][1] = "1."+dlin+":1234 " +kol+":22";
	objects[0][2] = "2." + dlin + ":1234 " + kol + ":22";
	objects[2][0] = "Continue: #";
	objects[2][1] = "Menu: *";
	// Print a message to the LCD.
	lcd.print("Input: ");
}

Menu::~Menu()
{
}

void Menu::UpdateProgRaw(LiquidCrystal lcd, int id, int leng, int amt)
{
	objects[0][id+1] = String(id)+"." + dlin +":"+ String(leng)+" "+ kol + ":"+String(amt);
}

void Menu::DrawMenu(LiquidCrystal lcd)
{
	lcd.setCursor(0, 1);
	lcd.print(objects[curX][curY + 1]);
	lcd.setCursor(0, 0);
	lcd.print(objects[curX][curY]);
}

void Menu::RunProg(LiquidCrystal lcd, int id, int leng, int amt) 
{
	lcd.clear();
	objects[1][0] = String(id)+"." + dlin + ":" + "0" + " " + iz + " " + String(leng);
	objects[1][1] = kol + ":" + "0" + " " + iz + " " + String(amt);
	curX = 1;
	curY = 0;
}

void Menu::StopProg(LiquidCrystal lcd)
{
	lcd.clear();
	curX = 2;
	curY = 0;
}

void Menu::MenuMode(LiquidCrystal lcd) 
{
	curX = 0;
	curY = 0;
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
	if (curY < maxY - 2) curY++;
}

void Menu::Up(LiquidCrystal lcd)
{
	lcd.clear();
	if (curY > 0) curY--;
}