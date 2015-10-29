#include "client.h"

void client::connect(tcp::resolver::iterator endpoint_iterator)
{
	boost::asio::async_connect(_socket, endpoint_iterator, 
		[this](boost::system::error_code ec, tcp::resolver::iterator){
		if (!ec) 
		{
			asio::async_read(_socket, asio::buffer(recv_buf.get()->data(), Packet::HEADER_LEN),
				strand.wrap(boost::bind(&client::read_header, this, asio::placeholders::error))
				);
		}
		else
		{
			std::cout << "error in connection" << ec << std::endl;
			exit(1);
		}
	});
}

void client::send(Packet::packet_ptr& p)
{
	std::wcout << L"send : " << p.get()->data() << std::endl;
	asio::async_write(_socket,
		boost::asio::buffer(p.get()->data(), p.get()->length()),
		strand.wrap(boost::bind(&client::handle_send, this,
		asio::placeholders::error))
	);
}

void client::handle_send(const boost::system::error_code & error) {
	if (!error)
	{
		std::cout << "send success" << std::endl;
	}
	else {
		std::cerr << "send fail : " << error.message() << std::endl;
	}
}

void client::read_header(const boost::system::error_code& error)
{
	Packet::Header header = Packet::ParseHeader(recv_buf);
	
	asio::async_read(_socket,
		boost::asio::buffer(recv_buf.get()->data() + 4, header.packet_len),
		strand.wrap(
			boost::bind(&client::handle_read_body,
			this,
			recv_buf,
			asio::placeholders::bytes_transferred,
			asio::placeholders::error)
		));
}

void client::handle_read_body(Packet::packet_ptr p, size_t byte_transfer, const boost::system::error_code & error)
{
	if (!error)
	{
		Packet::Parse(p);
		asio::async_read(_socket, asio::buffer(recv_buf.get()->data(), Packet::HEADER_LEN),
			strand.wrap(
				boost::bind(&client::read_header, this, asio::placeholders::error)
				)
			);
	}	
};

void client::dotest()
{
	Packet::test t;
	t.t = 10;
}

client::client(boost::asio::io_service& io_service, tcp::resolver::iterator endpoint_iterator)
	: _io_service(io_service), _socket(io_service), recv_buf(new Packet::Packet), strand(io_service)
{
	connect(endpoint_iterator);

}


client::~client()
{
}