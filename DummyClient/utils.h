#pragma once
#include <string>
#include <locale>
#include <codecvt>

namespace Utils {
	std::string Ws2S(const std::wstring&);
	std::wstring S2Ws(const std::string& str);
}
