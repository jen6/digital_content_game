#include "utils.h"

std::string Utils::Ws2S(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

std::wstring Utils::S2Ws(const std::string & str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

float Utils::GetTime()
{
	using Time = std::chrono::system_clock;
	using ms = std::chrono::milliseconds;
	using fsec = std::chrono::duration<float>;

	auto t0 = Time::now();
	auto time_ms = std::chrono::time_point_cast<ms>(t0);
	auto epoch = time_ms.time_since_epoch();
	auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
	auto t = fsec(value);
	return t.count();
}
