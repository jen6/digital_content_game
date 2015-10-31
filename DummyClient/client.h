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
	asio::strand strand;


	std::deque<Packet::packet_ptr> msg_queue;
	void connect(tcp::resolver::iterator );
	void read_header(const boost::system::error_code& error);
	void handle_read_body(Packet::packet_ptr p, size_t byte_transfer, const boost::system::error_code & error);
	void handle_send(const boost::system::error_code& error);
	void dotest();
public:
	client(asio::io_service& , tcp::resolver::iterator );
	void close();
	client() = delete;

	void send(Packet::packet_ptr& );
	void reconnect(tcp::resolver::iterator);
	~client();
};

