#include "client.h"



void client::connect()
{
	boost::asio::async_connect(_socket, endpoint_iterator, 
		[this](boost::system::error_code ec, tcp::resolver::iterator){
		if (!ec) 
		{
			dotest();
		}
	});
}

void client::send()
{
	boost::asio::async_write(_socket,
		boost::asio::buffer(msg_queue.front()), msg_queue.front().length());
}

void client::dotest()
{
	
}

//client::client()
//{
//}

client::client(std::string ip, std::string port) : _resolver(_io_service), _socket(_io_service)
{
	endpoint_iterator = _resolver.resolve({ ip, port });
}


client::~client()
{
}
