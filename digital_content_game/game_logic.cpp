#include "game_logic.h"

namespace game_logic 
{
	Packet::packet_ptr game_logic::do_work(std::shared_ptr<Packet::test> ptr)
	{
		std::cout << ptr.get()->t << std::endl;
		Packet::test t;
		t.t = 999;
		auto pack = t.Make_packet();
		return pack.shared_from_this();
	}
}