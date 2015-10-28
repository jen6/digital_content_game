#include "packet_parser.h"
namespace Packet 
{

	void Parse(packet_ptr p)
	{
		static int cnt = 0;
		UINT length, event;
		wchar_t * data_ptr = p.get()->data();
		std::memcpy(&event, reinterpret_cast<char *>(data_ptr), sizeof(UINT));
		std::memcpy(&length, reinterpret_cast<char *>(data_ptr) + 4, sizeof(UINT));
		Body_interface * ptr;
		packet_ptr ret;
			
		switch (static_cast<PACKET_EVENT>(event))
		{
		case PACKET_EVENT::TESTER:
			ptr = new test();
			try {
				ptr->Make_Body(data_ptr + HEADER_LEN / 2, length);
			}
			catch (std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
			cnt += 1;
			std::cout << "cnt : " << std::dec << cnt << std::endl;
			std::cout << "recved t : "<< std::dec << dynamic_cast<test*>(ptr)->t << std::endl;
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