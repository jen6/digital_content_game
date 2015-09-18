#pragma once
#include <cstdlib>
#include <boost/array.hpp>
#include "packet_events.h"

class Packet{
public:
	using OBJECT_IDX = unsigned int;
	
private:
	PACKET_EVENT pe;
	OBJECT_IDX ob_idx;

};

