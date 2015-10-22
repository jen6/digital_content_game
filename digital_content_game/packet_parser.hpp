#pragma once
#include "packet_structure.h"
#include "game_logic.h"
#include <cstdlib>
#include <sstream>
#include <mutex>
#include <thread>
#include <memory>

namespace Packet 
{
	template<class T>
	inline packet_ptr Parse(packet_ptr p, std::shared_ptr<T> room)
	{
		UINT length, event;
		char * data_ptr = p.get()->data();
		std::memcpy(&event, data_ptr, sizeof(UINT));
		std::memcpy(&length, data_ptr + 4, sizeof(UINT));
		Body_interface * ptr;
		packet_ptr ret;

		std::lock_guard<std::mutex> lock(room.get()->mtx);
		switch (event)
		{
		case static_cast<UINT>(PACKET_EVENT::TESTER) :
			ptr = new test();
			ptr->Make_Body(data_ptr + HEADER_LEN, length);
			ret = game_logic::do_work(std::shared_ptr<test>(dynamic_cast<test*>(ptr)));
			break;
			//case PACKET_EVENT::LOAD_INFO:
			//	InfoBody body(length, event);
			//	break;
			//case PACKET_EVENT::OBJECT_STATECH:
			//	StateBody body(length, event);
			//	break;
		default:
			std::cout << "no match" << std::endl;
			std::cout << "event : " << event << ",  tester : " << static_cast<UINT>(PACKET_EVENT::TESTER) << std::endl;
			break;
		}
		return ret;
	}
}