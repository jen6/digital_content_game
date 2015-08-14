#include "game_session.h"
#include <iostream>

session_ptr game_session::create(asio::io_service& _io_service)
{
	return session_ptr(new game_session(_io_service));
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

void game_session::send()
{
	_socket.async_write_some(
		asio::buffer(data_, data_.size()),
		boost::bind(
			&game_session::handle_write,
			this,
			asio::placeholders::error));
}

void game_session::handle_write(const boost::system::error_code& error)
{
	if (data_.front()) {
		_socket.async_write_some(
			asio::buffer(data_, data_.size()),
			boost::bind(
				&game_session::handle_read,
				this,
				asio::placeholders::error));
	}
	else {
		std::cout << "fuck!" << std::endl;
	}
	data_.fill(0);
}

game_session::~game_session()
{
}

game_session::game_session(asio::io_service & _io_service) : _socket(_io_service)
{
}

void game_session::handle_read(const boost::system::error_code & error)
{
	_socket.async_read_some(asio::buffer(data_, MAX_LENGTH),
		boost::bind(
			&game_session::handle_read,
			this,
			asio::placeholders::error));
	send();
}
