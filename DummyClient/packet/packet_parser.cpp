#include "packet_parser.h"
namespace Packet 
{
	void Parse(packet_ptr p)
	{
		UINT length, event;
		char * data_ptr = p.get()->data();
		std::memcpy(&event, data_ptr, sizeof(UINT));
		std::memcpy(&length, data_ptr + 4, sizeof(UINT));
		Body_interface * ptr;
		packet_ptr ret;

		switch (static_cast<PACKET_EVENT>(event))
		{
		case PACKET_EVENT::TESTER:
			ptr = new test();
			ptr->Make_Body(data_ptr + HEADER_LEN);
			std::cout << "recved t : " << dynamic_cast<test*>(ptr)->t << std::endl;
			break;
			//case PACKET_EVENT::LOAD_INFO:
			//	InfoBody body(length, event);
			//	break;
			//case PACKET_EVENT::OBJECT_STATECH:
			//	StateBody body(length, event);
			//	break;
		default:
			std::cout << " no match" << std::endl;
			break;
		}
	}
}