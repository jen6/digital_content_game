#pragma once
#include <boost\asio.hpp>
#include <string>
#include <deque>

using namespace boost::asio::ip;
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

	void dotest();
public:
	client(std::string ip, std::string port);
	~client();
};

