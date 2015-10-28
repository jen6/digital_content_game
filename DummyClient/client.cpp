#include "client.h"

void client::connect(tcp::resolver::iterator endpoint_iterator)
{
	boost::asio::async_connect(_socket, endpoint_iterator, 
		[this](boost::system::error_code ec, tcp::resolver::iterator){
		if (!ec) 
		{
		}
		else
		{
			std::cout << ec << std::endl;

		}
	});

	read();
}

void client::send(Packet::packet_ptr& p)
{
	std::cout << "hello" << std::endl;
	if (!p)
	{
		std::cout << "fuck! where is my ptr" << std::endl;
	}

	boost::function<void(const boost::system::error_code& )> f = 
		[](const boost::system::error_code & error) {
		if (error)
		{
			std::cout << error << std::endl;
		}
	};
	std::wcout << L"send : " << p.get()->data() << std::endl;
	_socket.async_write_some(
		boost::asio::buffer(p.get()->data(), p.get()->length()),
		boost::bind(f, 
			asio::placeholders::error)
	);
}


void client::read()
{
	_socket.async_read_some(
		boost::asio::buffer(recv_buf.get()->data(), Packet::MAX_LEN),
		//추후에 이부분에 핸들러 넣어주면 됨
		boost::bind(&client::handle_read,
			this,
			recv_buf,
			asio::placeholders::bytes_transferred,
			asio::placeholders::error)
	);
}

void client::handle_read(Packet::packet_ptr p, size_t byte_transfer, const boost::system::error_code & error)
{
	if (!error)
	{
		std::cout << "recv" << std::endl;
		Packet::Parse(p);
	}
	else
	{
		std::cout << "f" << std::endl;
	}
	_socket.async_read_some(
		boost::asio::buffer(recv_buf.get()->data(), Packet::MAX_LEN),
		//추후에 이부분에 핸들러 넣어주면 됨
		boost::bind(&client::read,
			this
			)
		);
};





void client::dotest()
{
	Packet::test t;
	t.t = 10;
}

client::client(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator)
	: _io_service(io_service), _socket(io_service), recv_buf(new Packet::Packet)
{
	connect(endpoint_iterator);

}


client::~client()
{
}