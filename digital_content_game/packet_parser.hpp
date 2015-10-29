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
	Header ParseHeader(wchar_t* ptr);

	//template<class T>
	//inline packet_ptr Parse(packet_ptr p, std::shared_ptr<T> room)
	packet_ptr Parse(packet_ptr p, room_ptr room);
}