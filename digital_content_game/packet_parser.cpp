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

	//template<class T>
	//inline packet_ptr Parse(packet_ptr p, std::shared_ptr<T> room)
	packet_ptr Parse(packet_ptr p, room_ptr room)
	{
		UINT length, event;
		wchar_t * data_ptr = p.get()->data();
		std::memcpy(&event, data_ptr, sizeof(UINT));
		std::memcpy(&length, reinterpret_cast<char*>(data_ptr) + 4, sizeof(UINT));
		Body_interface * ptr;
		packet_ptr ret;

		std::lock_guard<std::mutex> lock(room.get()->mtx);
		switch (static_cast<PACKET_EVENT>(event))
		{
		case PACKET_EVENT::TESTER:
			ptr = new test();
			ptr->Make_Body(data_ptr + HEADER_LEN / 2, length);
			ret = game_logic::do_work(std::shared_ptr<test>(dynamic_cast<test*>(ptr)));
			break;
		case PACKET_EVENT::OBJECT_MOVE:
			ptr = new MoveBody();
			ptr->Make_Body(data_ptr + HEADER_IDX, length);
			ret = game_logic::do_move(
				std::shared_ptr<MoveBody>(dynamic_cast<MoveBody*>(ptr)),
				room);
			break;
		default:
			std::cout << "no match" << std::endl;
			std::cout << "event : " << event << ",  tester : " << static_cast<UINT>(PACKET_EVENT::TESTER) << std::endl;
			break;
		}
		return ret;
	}
}