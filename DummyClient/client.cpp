#include "client.h"

void client::connect(const boost::system::error_code& error)
{
	boost::system::error_code ec;
	boost::asio::socket_base::keep_alive option(true);
	_socket.set_option(option, ec);
	if (ec) {
		std::cerr << ec.value() << std::endl;
		exit(0);
	}	

	if (!error) {
		_socket.async_read_some(
			boost::asio::buffer(recv_buf.get()->data(), Packet::MAX_LEN),
			//추후에 이부분에 핸들러 넣어주면 됨
			boost::bind(&client::read,
				this,
				asio::placeholders::error)
			);
	}
}
void client::handle_send(const boost::system::error_code & error) {
	static int ecnt = 0;
	static int scnt = 0;
	if (error)
	{
		ecnt += 1;
		std::cout << "send error : " << ecnt << std::endl;
		std::cout << "send error value : " << error.value() << std::endl;
	}
	else {
		scnt += 1;
		std::cout << "sendcnt : " << std::dec << (int)scnt << std::endl;
		try {
			std::cout << _socket.available() << std::endl;
		}
		catch(boost::system::system_error& e){
			std::cerr<<"error code : " << e.what() << std::endl;
		}

		_socket.async_read_some(
			boost::asio::buffer(recv_buf.get()->data(), Packet::MAX_LEN),
			//추후에 이부분에 핸들러 넣어주면 됨
			boost::bind(&client::read,
				this,
				asio::placeholders::error)
			);
	}
}

void client::handle_read_header(const boost::system::error_code& error)
{
	if (!error)
	{
		
	}
}

void client::send(Packet::packet_ptr& p)
{
	if (!_socket.is_open())
	{
		std::cout << " 소켓 죽음" << std::endl;
	}
	_socket.async_write_some(
		boost::asio::buffer(p.get()->data(), p.get()->length()),
		boost::bind(&client:: handle_send,
			this,
			asio::placeholders::error)
	);
}


void client::read(const boost::system::error_code & error)
{
	if (!_socket.is_open())
	{
		std::cout << " 소켓 죽음" << std::endl;
	}
	if (!error)
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
}

void client::handle_read(Packet::packet_ptr p, size_t byte_transfer, const boost::system::error_code & error)
{
	static int cnt = 0;
	if (!error)
	{
		cnt += 1;
		std::cout << "recv cnt : " << cnt << std::endl;
		Packet::Parse(p);
	}
	else
	{
		std::cout << "recv error number : " << error.value() << std::endl;
		std::cout << "f" << std::endl;
	}
	_socket.async_read_some(
		boost::asio::buffer(recv_buf.get()->data(), Packet::MAX_LEN),
		//추후에 이부분에 핸들러 넣어주면 됨
		boost::bind(&client::read,
			this,
			asio::placeholders::error)
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
	boost::asio::async_connect(_socket, endpoint_iterator,
		boost::bind(&client::connect,
			this, boost::asio::placeholders::error)
		);
}

client::~client()
{
}
