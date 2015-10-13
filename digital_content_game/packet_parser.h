#pragma once
#include "packet_structure.h"
#include "game_logic.h"
#include <cstdlib>
#include <sstream>

namespace Packet 
{
	packet_ptr Parse(packet_ptr p);
}