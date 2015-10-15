#pragma once
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\function.hpp>
#include <boost\shared_ptr.hpp>
#include <string>
#include "packet/packet.h"
#include <deque>
#include <string>



using namespace boost::asio::ip;
namespace asio = boost::asio;


class client
{
private:
	boost::asio::io_service& _io_service;
	tcp::socket _socket;
	std::deque<Packet::packet_ptr> msg_queue;
	void connect(tcp::resolver::iterator );
	void read();
	void dotest();
public:
	client(asio::io_service& , tcp::resolver::iterator );
	void send(Packet::packet_ptr& );
	~client();
};

