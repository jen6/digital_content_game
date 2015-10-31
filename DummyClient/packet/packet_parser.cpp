//이 파일 각각 case에다 처리할수있는 함수 넣어주면 됨.
#include "packet_parser.h"
#include <iostream>
namespace Packet 
{
	//헤더에서 패킷 종류와 길이 파싱
	Header ParseHeader(packet_ptr p)
	{
		UINT length, event;
		wchar_t * data_ptr = p.get()->data();
		auto Pdata = reinterpret_cast<char *>(data_ptr);

		std::memcpy(&event, Pdata, sizeof(UINT));
		std::memcpy(&length, Pdata + 4, sizeof(UINT));
		return Header(length, static_cast<PACKET_EVENT>(event));
	}

	//패킷 바디를 파싱
	void Parse(packet_ptr p, client& cle)
	{
		UINT length, event;
		wchar_t * data_ptr = p.get()->data();
		auto Pdata = reinterpret_cast<char *>(data_ptr);

		std::memcpy(&event, Pdata, sizeof(UINT));
		std::memcpy(&length,Pdata + 4, sizeof(UINT));
		Body_interface * ptr;
		packet_ptr ret;
			
		switch (static_cast<PACKET_EVENT>(event))
		{
		case PACKET_EVENT::TESTER:
			ptr = new test();
			try {
				ptr->Make_Body(data_ptr + HEADER_IDX, length);
			}
			catch (std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
			std::cout << "recved t : "<< std::dec << dynamic_cast<test*>(ptr)->t << std::endl;
			break;

		case PACKET_EVENT::USER_INFO:
			ptr = new UserInfoBody();
			ptr->Make_Body(data_ptr + HEADER_IDX, length);
			cle.user = dynamic_cast<UserInfoBody *>(ptr);
			break;

		case PACKET_EVENT::SESSION_NO_MATCH:
			std::cout << "no session match" << std::endl;
			cle.close();
			//세션 안맞을때 처리할 방법
			break;

		case PACKET_EVENT::OBJECT_MOVE:
			ptr = new MoveBody();
			try {
				ptr->Make_Body(data_ptr + HEADER_IDX, length);
			}
			catch (std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
			std::cout << "recved move idx :  "
				<< dynamic_cast<MoveBody*>(ptr)->object_idx
				<<"\n- x, y : "
				<<  dynamic_cast<MoveBody*>(ptr)->x
				<< ", " << dynamic_cast<MoveBody*>(ptr)->y << std::endl;
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