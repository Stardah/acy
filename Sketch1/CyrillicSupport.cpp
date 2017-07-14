#include "CyrillicSupport.h"

char mapCyrillic(char character)
{
    using _u = unsigned char;
    static const int CharactersTotal = 256;
    static char map[CharactersTotal] = { 0 };
    static bool nonInit = true;

    if (nonInit)
    {
        map[_u('�')] = char('A');	map[_u('�')] = char('a');
        map[_u('�')] = char(160);	map[_u('�')] = char(178);
        map[_u('�')] = char('B');	map[_u('�')] = char(179);
        map[_u('�')] = char(161);	map[_u('�')] = char(180);
        map[_u('�')] = char(224);	map[_u('�')] = char(227);
        map[_u('�')] = char('E');	map[_u('�')] = char('e');
        map[_u('�')] = char(162);	map[_u('�')] = char(181);
        map[_u('�')] = char(163);	map[_u('�')] = char(182);
        map[_u('�')] = char(164);	map[_u('�')] = char(183);
        map[_u('�')] = char(165);	map[_u('�')] = char(184);
        map[_u('�')] = char(166);	map[_u('�')] = char(185);
        map[_u('�')] = char('K');	map[_u('�')] = char(186);
        map[_u('�')] = char(167);	map[_u('�')] = char(187);
        map[_u('�')] = char('M');	map[_u('�')] = char(188);
        map[_u('�')] = char('H');	map[_u('�')] = char(189);
        map[_u('�')] = char('O');	map[_u('�')] = char('o');
        map[_u('�')] = char(168);	map[_u('�')] = char(190);
        map[_u('�')] = char('P');	map[_u('�')] = char('p');
        map[_u('�')] = char('C');	map[_u('�')] = char('c');
        map[_u('�')] = char('T');	map[_u('�')] = char(191);
        map[_u('�')] = char(169);	map[_u('�')] = char('y');
        map[_u('�')] = char(170);	map[_u('�')] = char(228);
        map[_u('�')] = char('X');	map[_u('�')] = char('x');
        map[_u('�')] = char(229);	map[_u('�')] = char(229);
        map[_u('�')] = char(171);	map[_u('�')] = char(192);
        map[_u('�')] = char(172);	map[_u('�')] = char(193);
        map[_u('�')] = char(226);	map[_u('�')] = char(230);
        map[_u('�')] = char(173);	map[_u('�')] = char(194);
        map[_u('�')] = char(174);	map[_u('�')] = char(195);
        map[_u('�')] = char('b');	map[_u('�')] = char(196);
        map[_u('�')] = char(175);	map[_u('�')] = char(197);
        map[_u('�')] = char(176);	map[_u('�')] = char(198);
        map[_u('�')] = char(177);	map[_u('�')] = char(199);
        nonInit = false;
    }

    char rv = map[(unsigned char)character];
    return rv ? rv : character;
}

String operator ""_Cyr(char const* str, std::size_t size)
{
    static const int BufferSize = 256;
    char buffer[BufferSize];

    char const* src = str;
    char* dst = buffer;

    for (std::size_t i = 0; i < size; ++i)
    {
        *dst++ = mapCyrillic(*src++);
    }

    *dst = '\0';

    return String(buffer);
}