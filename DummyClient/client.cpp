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
	_socket.async_write_some(
		boost::asio::buffer(msg_queue.front().get()->data(), msg_queue.front().get()->length()),
		boost::bind([](const boost::system::error_code & error) {
		std::cout << "send" << std::endl;
		}
		,asio::placeholders::error)
	);
}

void client::read()
{
	shared_Packet p;
	_socket.async_read_some(
		boost::asio::buffer(p.get()->data(), Packet::MAX_LEN),
		boost::bind([](shared_Packet p, size_t byte_transfer, const boost::system::error_code & error) {
			std::cout << "recv" << std::endl;
			
		},
			p,
			asio::placeholders::bytes_transferred,
			asio::placeholders::error)
	);
}



void client::dotest()
{
	
}

client::client(std::string ip, std::string port) : _resolver(_io_service), _socket(_io_service)
{
	endpoint_iterator = _resolver.resolve({ ip, port });
}


client::~client()
{
}
