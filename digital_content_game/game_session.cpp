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
		char a[4];
		int body_len;
		std::memcpy(a, data_.c_array(), 4);
		body_len = std::atoi(a);
		std::cout << "len : " << body_len << std::endl;
		char * pt = data_.c_array() + 4 + body_len;
		*pt = 0;
		std::cout << &data_[4] << std::endl;
		asio::async_write(
			_socket,
			asio::buffer(data_, 4 + body_len),
			boost::bind(
				&game_session::handle_write,
				this,
				asio::placeholders::error));
	}
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

game_session::game_session(asio::io_service & _io_service) : _socket(_io_service)
{
}

