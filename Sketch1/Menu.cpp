#include "Menu.h"

const String notification[] = { "УП записана","Ожидание резки","УП выполнена", "Ошибка" };

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
	lcd.print(objects[curX][curY + 1]);
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
	if (curY < maxY - 2) curY++;
}

void Menu::Up(LiquidCrystal lcd)
{
	lcd.clear();
	if (curY > 0) curY--;
}