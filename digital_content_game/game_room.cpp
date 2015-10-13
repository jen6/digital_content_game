#include "game_room.h"
#include <iostream>


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
		std::cout << "session established!" << std::endl;
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
	for (auto user : users)
	{
		user.get()->send(p);
	}
}

void game_room::PassTask(std::function<Packet::packet_ptr()> task)
{
	auto packet_fut = pool.get()->enqueue(task);
	auto packet = packet_fut.get();
	broadcast(packet);
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
	_socket.async_read_some(asio::buffer(data_, MAX_LENGTH),
		boost::bind(
			&game_session::handle_read,
			this,
			asio::placeholders::error
			));
}

void game_session::send(Packet::packet_ptr p)
{
	_socket.async_write_some(
		asio::buffer(p.get()->data(), p.get()->length()),
		boost::bind(
			&game_session::handle_write,
			this,
			asio::placeholders::error));
}

void game_session::broadcast(Packet::packet_ptr p)
{
	_game_room.broadcast(p);
}

void game_session::handle_read(const boost::system::error_code & error)
{
	if (!error)
	{
		_socket.async_read_some(asio::buffer(data_, MAX_LENGTH),
			boost::bind(
				&game_session::handler,
				this,
				asio::placeholders::error,
				asio::placeholders::bytes_transferred));
	}
}

//TODO 여기부분 패킷파서 만들기
void game_session::handler(const boost::system::error_code & error, std::size_t recv_size)
{
	if (!error)
	{
		boost::array<char, 1024> buf(data_);
		auto ptr = Packet::Packet(buf).shared_from_this();
		std::function<Packet::packet_ptr()> task(std::bind(&Packet::Parse, ptr));
		_game_room.PassTask(task);
	}
	_socket.async_read_some(asio::buffer(data_, MAX_LENGTH),
		boost::bind(
			&game_session::handle_read,
			this,
			asio::placeholders::error
			));
}
void game_session::handle_write(const boost::system::error_code& error)
{
	if (!error) {
		_socket.async_read_some(asio::buffer(data_, MAX_LENGTH),
			boost::bind(
				&game_session::handle_read,
				this,
				asio::placeholders::error));
	}
}

game_session::~game_session()
{
}

game_session::game_session(asio::io_service & _io_service, game_room& room) : _socket(_io_service), _game_room(room)
{
}


