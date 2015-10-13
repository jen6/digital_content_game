#include "game_session.h"
#include <iostream>

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
	if (!error){ 
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

