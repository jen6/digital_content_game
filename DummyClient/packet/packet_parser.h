#pragma once
#include "packet_structure.h"

#include <cstdlib>
#include <sstream>

#include "..\client_sock.h"


class ClientSock;

namespace Packet 
{
	Header ParseHeader(packet_ptr p);
	void Parse(packet_ptr p, ClientSock& cle);
}