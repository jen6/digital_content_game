#include "packet_parser.h"
namespace Packet 
{
	packet_ptr Parse(packet_ptr p)
	{
		UINT length, event;
		char * data_ptr = p.get()->data();
		std::memcpy(&length, data_ptr, sizeof(UINT));
		std::memcpy(&event, data_ptr + 4, sizeof(UINT));
		Body_interface * ptr;
		packet_ptr ret;

		switch (static_cast<PACKET_EVENT>(event))
		{
		case PACKET_EVENT::TESTER:
			ptr = new test();
			ptr->Make_Body(data_ptr + HEADER_LEN);
			ret = game_logic::do_work(std::shared_ptr<test>(dynamic_cast<test*>(ptr)));
			break;
			//case PACKET_EVENT::LOAD_INFO:
			//	InfoBody body(length, event);
			//	break;
			//case PACKET_EVENT::OBJECT_STATECH:
			//	StateBody body(length, event);
			//	break;
		default:
			break;
		}
		return ret;
	}
}