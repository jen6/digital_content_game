#include <iostream>
#include "game_room.h"



room_ptr game_room::create(asio::io_service & io, unsigned short port_num, ThreadPoolPtr tp)
{
	return room_ptr(new game_room(io, port_num, tp));
}

game_room::game_room(asio::io_service &_io_service, unsigned short port_num, ThreadPoolPtr tp)
	: _acceptor(_io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_num)),
	pool(tp)
{
	start_accept();
}


game_room::~game_room()
{
	
}

void game_room::start_accept()
{
	session_ptr ptr = game_session::create(_acceptor.get_io_service(), *this);
	_acceptor.async_accept(ptr.get()->socket(),
		boost::bind(&game_room::handle_accept, this, ptr, asio::placeholders::error));
}

void game_room::handle_accept(session_ptr ptr, const boost::system::error_code& error) 
{
	if (!error)
	{
		std::lock_guard<std::mutex> lock(mtx);
		users.push_back(ptr);
		ptr.get()->start();
		start_accept();
	}
	else {
		ptr.reset();
	}
}

void game_room::broadcast(Packet::packet_ptr p)
{
	std::lock_guard<std::mutex> lock(mtx);
	for (auto user : users)
	{
		user.get()->send(p);
	}
}

void game_room::PassTask(std::function<Packet::packet_ptr()> task)
{
	auto packet_fut = pool.get()->enqueue(task);
	try {
		packet_fut.wait();
		auto packet = packet_fut.get();
		if (packet)
		{
			broadcast(packet);
		}
		else
		{
			Log::Logger::Instance()->L("error in pass task");
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	
}



session_ptr game_session::create(asio::io_service& _io_service, game_room& room)
{
	return session_ptr(new game_session(_io_service, room));
}

asio::ip::tcp::socket & game_session::socket()
{
	return _socket;
}

void game_session::start()
{
	std::string str = _socket.remote_endpoint().address().to_string();
	Log::Logger::Instance()->L("client connected session : " + str);

	asio::async_read(_socket,
		asio::buffer(data_, Packet::HEADER_LEN),
		boost::bind(
			&game_session::read_header,
			this,
			asio::placeholders::error
		)
	);
}

void game_session::close()
{
	boost::system::error_code errorcode;

	if (_socket.is_open())
	{
		_socket.close(errorcode);
		if (errorcode)
		{
			Log::Logger::Instance()->L("socket.close error: " + errorcode.message());
		}
		else
		{
			Log::Logger::Instance()->L("socket closed");
		}
	}
}

void game_session::read_header(const boost::system::error_code& error)
{
	if (!error)
	{
		Packet::Header header = Packet::ParseHeader(data_.data());

		std::cout << "Parsed : " << static_cast<unsigned int>(header.packet_event) << ", " << header.packet_len << std::endl;

		auto ptr = reinterpret_cast<char*>(data_.data()) + Packet::HEADER_LEN;

		asio::async_read(_socket,
			boost::asio::buffer(ptr, header.packet_len),
			strand.wrap(
				boost::bind(&game_session::handle_read_body,
					this,
					asio::placeholders::error)
				));
	}
	else if (error == boost::asio::error::eof)
	{
		std::string str = _socket.remote_endpoint().address().to_string();
		Log::Logger::Instance()->L("client disconnected session : " + str);
	}
}

void game_session::handle_read_body(const boost::system::error_code & error)
{
	if (!error)
	{
		boost::array<wchar_t, 1024> buf(data_);

		auto ptr = std::make_shared<Packet::Packet>(buf);
		std::function<Packet::packet_ptr()> task(
			std::bind(&Packet::Parse,
				ptr, _game_room.shared_from_this()
				)
		);

		_game_room.PassTask(task);

		asio::async_read(_socket,
			asio::buffer(data_, Packet::HEADER_LEN),
			boost::bind(
				&game_session::read_header,
				this,
				asio::placeholders::error));
	}
	else {
		std::string str = _socket.remote_endpoint().address().to_string();
		Log::Logger::Instance()->L("client error read body session : " + str);
		std::cout << "err cnt : " << error.message() << std::endl;
	}
}

void game_session::send(Packet::packet_ptr p)
{
	asio::async_write(
		_socket,
		asio::buffer(p.get()->data(), p.get()->length()),
		strand.wrap(boost::bind(
		&game_session::handle_write,
		this,
		asio::placeholders::error)));
}

void game_session::handle_write(const boost::system::error_code& error)
{
	if (error)
	{
		//std::cout << "error in write : " << error.message() << " " << error.value() << std::endl;
		std::string str = _socket.remote_endpoint().address().to_string();
		Log::Logger::Instance()->L("client error in send session : " + str);
	}
}

void game_session::broadcast(Packet::packet_ptr p)
{
	_game_room.broadcast(p);
}

game_session::~game_session()
{
	close();
}

game_session::game_session(asio::io_service & _io_service, game_room& room) : _socket(_io_service), _game_room(room), strand(_io_service)
{
}