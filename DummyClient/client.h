#pragma once
#include <boost\asio.hpp>
#include <boost\bind.hpp>
#include <boost\shared_ptr.hpp>
#include <string>
#include <deque>
#include "../digital_content_game/packet_structure.h"


using namespace boost::asio::ip;
namespace asio = boost::asio;

using shared_Packet = boost::shared_ptr<Packet::Packet>;

class client
{
private:
	boost::asio::io_service _io_service;
	tcp::resolver _resolver;
	tcp::endpoint _end_point;
	tcp::resolver::iterator endpoint_iterator;
	tcp::socket _socket;
	std::deque<std::string> msg_queue;
	void connect();
	void send();
	void read();
	void dotest();
public:
	client(std::string ip, std::string port);
	~client();
};

