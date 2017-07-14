#pragma once

#include "WString.h"
#include <utility>

char mapCyrillic(char character);
String operator ""_Cyr(char const* str, std::size_t size);