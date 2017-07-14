#include "CyrillicSupport.h"

char mapCyrillic(char character)
{
    using _u = unsigned char;
    static const int CharactersTotal = 256;
    static char map[CharactersTotal] = { 0 };
    static bool nonInit = true;

    if (nonInit)
    {
        map[_u('À')] = char('A');	map[_u('à')] = char('a');
        map[_u('Á')] = char(160);	map[_u('á')] = char(178);
        map[_u('Â')] = char('B');	map[_u('â')] = char(179);
        map[_u('Ã')] = char(161);	map[_u('ã')] = char(180);
        map[_u('Ä')] = char(224);	map[_u('ä')] = char(227);
        map[_u('Å')] = char('E');	map[_u('å')] = char('e');
        map[_u('¨')] = char(162);	map[_u('¸')] = char(181);
        map[_u('Æ')] = char(163);	map[_u('æ')] = char(182);
        map[_u('Ç')] = char(164);	map[_u('ç')] = char(183);
        map[_u('È')] = char(165);	map[_u('è')] = char(184);
        map[_u('É')] = char(166);	map[_u('é')] = char(185);
        map[_u('Ê')] = char('K');	map[_u('ê')] = char(186);
        map[_u('Ë')] = char(167);	map[_u('ë')] = char(187);
        map[_u('Ì')] = char('M');	map[_u('ì')] = char(188);
        map[_u('Í')] = char('H');	map[_u('í')] = char(189);
        map[_u('Î')] = char('O');	map[_u('î')] = char('o');
        map[_u('Ï')] = char(168);	map[_u('ï')] = char(190);
        map[_u('Ð')] = char('P');	map[_u('ð')] = char('p');
        map[_u('Ñ')] = char('C');	map[_u('ñ')] = char('c');
        map[_u('Ò')] = char('T');	map[_u('ò')] = char(191);
        map[_u('Ó')] = char(169);	map[_u('ó')] = char('y');
        map[_u('Ô')] = char(170);	map[_u('ô')] = char(228);
        map[_u('Õ')] = char('X');	map[_u('õ')] = char('x');
        map[_u('Ö')] = char(229);	map[_u('ö')] = char(229);
        map[_u('×')] = char(171);	map[_u('÷')] = char(192);
        map[_u('Ø')] = char(172);	map[_u('ø')] = char(193);
        map[_u('Ù')] = char(226);	map[_u('ù')] = char(230);
        map[_u('Ú')] = char(173);	map[_u('ú')] = char(194);
        map[_u('Û')] = char(174);	map[_u('û')] = char(195);
        map[_u('Ü')] = char('b');	map[_u('ü')] = char(196);
        map[_u('Ý')] = char(175);	map[_u('ý')] = char(197);
        map[_u('Þ')] = char(176);	map[_u('þ')] = char(198);
        map[_u('ß')] = char(177);	map[_u('ÿ')] = char(199);
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