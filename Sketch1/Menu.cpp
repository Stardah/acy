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

String dlin = D+l+si+n+": ";
String kol = K+o+l+": ";
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

	// Close curX = 2
	items[2][0] = P+r+o+d+o+l+zh+si+t+smyagkiy+':'+' '+'#';
	items[2][1] = M+e+n+u+':'+' '+'*';
	// Print a message to the LCD.
	lcd.print("  "+Z+a+g+r+y+z+k+a+"...");
}

void Menu::UpdateProgRaw(int leng, int amt)
{
	items[0][0] = dlin + String(leng);
	items[0][1] = kol + String(amt);
}

String str = "";
void Menu::DrawMenu()
{
	switch (menuMode)
	{
	/*case Service: // +'>' before selected line
		lcd.setCursor(0, 0);
		if (upside) str = ">";
		lcd.print(str+items[curX][curY]);
		lcd.setCursor(0, 1);
		str = "";
		if (!upside) str = ">";
		lcd.print(str+items[curX][curY + 1]);
		break;
		*/
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

String inp[12];
void Menu::DrawService(int inputs[12], int encoderCounter)
{
	
	for(int i =0; i< 12; i++)
		if (inputs[i]==1) inp[i] = "1"; 
		else inp[i] = "0";
	
	items[3][0]=
		"FR1" + inp[0] +
		" FR2" +inp[1] +// 9 
		" HD1" +inp[2]; // 14
	items[3][1]=
		"HD2" + inp[3] + // 4
		" EM" + inp[4] + // 8
		" AU" + inp[5] + // 12
		" KN" + String(inputs[6]); // 16
	items[3][2]=
		"GF"  + inp[7] +// 3
		" GS" + inp[8] +// 7
		" SO" + inp[9] +// 11
		" E" + inp[10]+ inp[11]; // 15
	items[3][3] = String(encoderCounter);
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(items[3][curY]);
	lcd.setCursor(0, 1);
	lcd.print(items[3][curY + 1]);
}

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
	case Stop:
		lcd.clear();
		curX = 2;
		curY = 0;
		break;
	case Service:
		lcd.clear();
		curX = 3;
		curY = 0;
		break;
	case Run:
		break;
	default:
		break;
	}
}

void Menu::ApplyInput(int &leng, int &amt)
{
	leng = items[0][0].substring(6).toInt();
	amt = items[0][1].substring(5).toInt();
	length = leng;	// Set length
	parts = amt;	// Set parts
}

///
///
///
void Menu::Notification(int i)
{
	lcd.clear();
	lcd.setCursor(0, 0);
	//lcd.print(notification[i]);
}

void Menu::Input(char cha)
{
	if (upside && items[curX][curY].length() < 10 )// "dlin: " - 6 + 4 numbers
		items[curX][curY] += cha;
	if (!upside && items[curX][curY + 1].length() < 7)	 // "kol: " - 5 + 2 numbers
		items[curX][curY+1] += cha;	
}

///
/// Delete last symbol
///
void Menu::DelLast()
{
	if (upside && items[curX][curY].length() > 6) //"dlin: " - 6
	{
		items[curX][curY] = items[curX][curY].substring(0, items[curX][curY].length() - 1);
		lcd.clear();
	}
	if(!upside && items[curX][curY+1].length() > 5) // "kol: "- 5
	{
		items[curX][curY+1] = items[curX][curY+1].substring(0, items[curX][curY+1].length() - 1);
		lcd.clear();
	}
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

int Menu::getX() const
{
	return curX;
}

int Menu::getY() const
{
	return curY;
}

void Menu::Toast(String str)
{
	lcd.setCursor(0, 0);
	lcd.print(str);
}



