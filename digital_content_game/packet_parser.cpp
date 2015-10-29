#include "packet_parser.hpp"
namespace Packet 
{
	Header ParseHeader(wchar_t * ptr)
	{
		UINT length, event;
		wchar_t * data_ptr = ptr;
		std::memcpy(&event, reinterpret_cast<char *>(data_ptr), sizeof(UINT));
		std::memcpy(&length, reinterpret_cast<char *>(data_ptr) + 4, sizeof(UINT));
		return Header(length, static_cast<PACKET_EVENT>(event));
	}
}