#pragma once
#include <deque>
#include <string>
#include <thread>

#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\function.hpp>
#include <boost\shared_ptr.hpp>

#include "packet/packet.h"


using namespace boost::asio::ip;
namespace asio = boost::asio;


class client
{
private:
	Packet::packet_ptr recv_buf;

	boost::asio::io_service& _io_service;
	tcp::socket _socket;
	std::deque<Packet::packet_ptr> msg_queue;
	void connect(tcp::resolver::iterator );
	void read();
	void handle_read(Packet::packet_ptr p, size_t byte_transfer, const boost::system::error_code & error);
	void dotest();
public:
	client(asio::io_service& , tcp::resolver::iterator );
	client() = delete;
	void send(Packet::packet_ptr& );
	~client();
};

