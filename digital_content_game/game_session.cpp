#include "game_session.h"
#include <iostream>
#include <cstdlib>
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
	_socket.async_read_some(asio::buffer(msg.data(), MAX_LENGTH),
		boost::bind(
			&game_session::handle_read, 
			this, 
			asio::placeholders::error
			));
}

void game_session::send()
{
	_socket.async_write_some(
		asio::buffer(msg, data_.size()),
		boost::bind(
			&game_session::handle_write,
			this,
			asio::placeholders::error));
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

void game_session::handler(const boost::system::error_code & error,  std::size_t recv_size)
{
	if (!error)
	{
		std::cout << data_.c_array() << std::endl;
		asio::async_write(
			_socket, 
			asio::buffer(data_.c_array(), std::strlen(data_.c_array())),
			boost::bind(
				&game_session::handle_write, 
				this, 
				asio::placeholders::error));
	}
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

game_session::game_session(asio::io_service & _io_service) : _socket(_io_service)
{
}

