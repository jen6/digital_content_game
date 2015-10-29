#pragma once
#include "packet_structure.h"

#include <cstdlib>
#include <sstream>

namespace Packet 
{
	Header ParseHeader(packet_ptr p);
	void Parse(packet_ptr p);
}