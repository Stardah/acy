#include "Menu.h"
#include <locale.h>

#define A String('A')
#define B String(char(160))
#define V String('B')
#define G String(char(161))
#define D String(char(224))
#define E String('E')
#define Yo String(char(162))
#define Zh String(char(163))
#define Z String(char(164))
#define I String(char(165))
#define Ii String(char(166))
#define K String('K')
#define L String(char(167))
#define M String('M')
#define N String('H')
#define O String('O')
#define P String(char(168))
#define R String('P')
#define S String('C')
#define T String('T')
#define Y String(char(169))
#define F String(char(170))
#define X String('X')
#define Tz String(char(229))
#define Ch String(char(171))
#define Sh String(char(172))
#define Shi String(char(226))
#define tverdiy String(char(173))
#define Ui String(char(174))
#define myagkiy String('b')
#define Ee String(char(175))
#define Yu String(char(176))
#define Ya String(char(177))

#define a String('a')
#define b String(char(178))
#define v String(char(179))
#define g String(char(180))
#define d String(char(227))
#define e String('e')
#define yo String(char(181))
#define zh String(char(182))
#define z String(char(183))
#define si String(char(184))
#define ii String(char(185))
#define k String(char(186))
#define l String(char(187))
#define m String(char(188))
#define n String(char(189))
#define o String('o')
#define p String(char(190))
#define r String('p')
#define s String('c')
#define t String(char(191))
#define y String('y')
#define f String(char(228))
#define x String('x')
#define tz String(char(229))
#define ch String(char(192))
#define sh String(char(193))
#define shi String(char(230))
#define stverdiy String(char(194))
#define ui String(char(195))
#define smyagkiy String(char(196))
#define ee String(char(197))
#define u String(char(198))
#define ya String(char(199))

String dlin = D+l+si+n+a+": ";
String kol = K+o+l+"-"+v+o+": ";
String iz = I+z;


Menu::Menu(const LiquidCrystal& lcdInit) :
	lcd(lcdInit)
{
	lcd.begin(16,2);
	for (size_t i = 0; i < maxX; i++)
	{
		for (size_t j = 0; j < maxY; j++)
		{
			if (j != maxY - 1)
				items[i][j] = "               ";
			else 
				items[i][j] = "|______________|";
		}
	}

	// Notifications
	notification[0] = ""+A+v+a+r+". "+o+s+t+a+n+o+v+"!";
	notification[1] ="    "+ O + t + r + e + zh + t + e+"!";

	// Close curX = 2
	items[2][0] = P+r+o+d+o+l+zh+si+t+smyagkiy+':'+' '+'#';
	items[2][1] = P+r+o+g+r+a+m+m+a+':'+' '+'*';
	// Service
	items[3][0] = T + o + ch + n + o + s + t + smyagkiy + " = ";
	items[3][1] = Z + a + d + e + r + zh + k + a + " = ";
	// Notify curX = 4
	items[4][0] = "    "+V+n+si+m+a+n+si+e+":";
	items[4][1] = "     "+O+k+ " (D)";
	// Print a message to the LCD.
	lcd.print("  "+Z+a+g+r+y+z+k+a+"...");
}

void Menu::UpdateValues(int leng, int amt)
{
	if (menuMode == Menus::Inp)
	{
		items[curX][0] = dlin + String(leng);
		items[curX][1] = kol + String(amt);
	}
	else // Service
	{
		items[curX][0] = T + o + ch + n + o + s + t + smyagkiy + " = " + String(leng);
		items[curX][1] = Z + a + d + e + r + zh + k + a + " = " + String(amt);
	}
}

void Menu::DrawMenu()
{
	switch (menuMode)
	{
	case Inp: // cursor after selected line
		if (upside) 
		{
			lcd.setCursor(0, 1);
			lcd.print(items[curX][1]);
			lcd.setCursor(0, 0);
			lcd.print(items[curX][0]);
		}
		else
		{
			lcd.setCursor(0, 0);
			lcd.print(items[curX][0]);
			lcd.setCursor(0, 1);
			lcd.print(items[curX][1]);
		}
		break;
	default:
		lcd.setCursor(0, 0);
		lcd.print(items[curX][curY]);
		lcd.setCursor(0, 1);
		lcd.print(items[curX][curY + 1]);
		break;
	}
}

///
/// Draw service menu
///
String inp[12];
void Menu::DrawService(int inputs[12], int encoderCounter)
{
	
	for(int i =0; i< 12; i++)
		if (inputs[i]==1) inp[i] = "1"; 
		else inp[i] = "0";
	
		items[3][2] =
			"FR1 = " + inp[0] +
			" FR2 = " + inp[1];// 9 
		items[3][3]	= 
			"HD1 = " + inp[2] + // 14
			" HD2 = " + inp[3];
		items[3][4] =
			A+v+a+r+"= " + inp[4] +
			" "+A+v+t+o+"= " + inp[5];
		items[3][5] =
			N + o + zh + " = " + inp[6] +
			" EAB=" + inp[10] + ":" +inp[11];
		items[3][6] = Ee + n+k+o+d+e+r+" = "+ String(encoderCounter);
	lcd.clear();

	if (upside)
	{
		lcd.setCursor(0, 1);
		lcd.print(items[curX][curY + 1]);
		lcd.setCursor(0, 0);
		lcd.print(items[curX][curY]);
	}
	else
	{
		lcd.setCursor(0, 0);
		lcd.print(items[curX][curY]);
		lcd.setCursor(0, 1);
		lcd.print(items[curX][curY + 1]);
	}
}

///
/// Draw run mode menu
///
void Menu::RTUpdate(int curlength, int curparts)
{
	if (curX == 1) 
	{
		lcd.clear();
		items[1][1] = T + e + k + ": " + String(curlength);
		if (String(curlength).length()<=4) for (int i = 0; i <= 4 - String(curlength).length(); i++)
		{
			items[1][1] += " ";
		}
		items[1][1] += T + e + k + ":" + String(curparts);
	}
}

///
/// Switch to run menu
///
void Menu::RunProg(int leng, int amt) 
{
	lcd.clear();
	items[1][0] = D + l + si + n + ":" + String(leng);
	items[1][1] = T + e + k + ": " + "0";
	for (int i = 0; i <= 4 - String(leng).length(); i++)
	{
		items[1][0] += " ";
		items[1][1] += " ";
	}
	items[1][0]	+= K+o+l+":"+ String(amt);
	items[1][1] += T + e + k + ":"+"0";
	curX = 1;
	curY = 0;
	menuMode = Menus::Run;
}

///
/// Switch to chosen menu mode
///
void Menu::SetMenuMode(int newMenu)
{
	upside = true;
	menuMode = Menus(newMenu);
	switch (newMenu)
	{
	case Inp:
		upside = true;
		lcd.clear();
		curX = 0;
		curY = 0;
		break;
	case Service:
		upside = true;
		lcd.clear();
		curX = 3;
		curY = 0;
		break;
	case Notify:
		lcd.clear();
		curX = 4;
		curY = 0;
		break;
	case Run:
		break;
	default:
		break;
	}
}

///
/// Update input variables in main class
///
void Menu::ApplyInput(int &leng, int &amt)
{
	if (menuMode == Menus::Inp)
	{
		leng = items[0][0].substring(7).toInt();
		amt = items[0][1].substring(8).toInt();
		length = leng;	// Set length
		parts = amt;	// Set parts
	}
	else // Service
	{
		leng = items[curX][0].substring(11).toInt();
		amt = items[curX][1].substring(11).toInt();
	}
}

///
/// Notifications
///
void Menu::Notification(int i)
{
	lcd.clear();
	lcd.setCursor(0, 0);
	if (i == 0)
	{
		lcd.print("   "+O+s+t+a+n+o+v);
		lcd.setCursor(0, 1);
		lcd.print("  "+a+k+t+si+v+si+r+o+v+a+n+"!");
	}
	else {
		lcd.print(items[4][0]);
		lcd.setCursor(0, 1);
		lcd.print(notification[i]);
	}
}

///
/// Input char
///
void Menu::Input(char cha)
{
		if (menuMode == Menus::Inp)
		{
			if (upside && items[curX][curY].length() < 11)// "dlin: " - 7 + 4 numbers
				items[curX][curY] += cha;
			if (!upside && items[curX][curY + 1].length() < 10)	 // "kol: " - 8 + 2 numbers
				items[curX][curY + 1] += cha;
		}
		else if (curY<2) // Service
		{
			if (upside && curY == 1 && items[curX][curY].length() < 15)
				items[curX][curY] += cha;
			else
			if (upside && items[curX][curY].length() < 13)
				items[curX][curY] += cha;
			else
			if (!upside && items[curX][curY + 1].length() < 15)	 
				items[curX][curY + 1] += cha;
		}
}

///
/// Delete last symbol
///
void Menu::DelLast()
{
	if (menuMode == Menus::Inp)
	{
		if (upside && items[curX][curY].length() > 7) //"dlina: " - 7
		{
			items[curX][curY] = items[curX][curY].substring(0, items[curX][curY].length() - 1);
			lcd.clear();
		}
		if (!upside && items[curX][curY + 1].length() > 8) // "kol-vo: "- 8
		{
			items[curX][curY + 1] = items[curX][curY + 1].substring(0, items[curX][curY + 1].length() - 1);
			lcd.clear();
		}
	}
	else if (curY<2) // Service
	{
		if (upside && items[curX][curY].length() > 11) //"dlin: " - 6
		{
			items[curX][curY] = items[curX][curY].substring(0, items[curX][curY].length() - 1);
		}
		if (!upside && items[curX][curY + 1].length() > 11) // "kol: "- 5
		{
			items[curX][curY + 1] = items[curX][curY + 1].substring(0, items[curX][curY + 1].length() - 1);
		}
	}
}

void Menu::Down()
{
	lcd.clear();
	if (upside) upside = !upside;
		else 
			if (menuMode == Menus::Service)
			if (curY < maxY - 1)
				++curY;
}

void Menu::Up()
{
	lcd.clear();
	if (!upside) upside = !upside;
	else 
		if (menuMode == Menus::Service)
		if (curY > 0)
			--curY;
}


