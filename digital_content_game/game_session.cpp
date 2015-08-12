#include "game_session.h"




session_ptr game_session::create(asio::io_service& _io_service)
{
	return session_ptr(new game_session(_io_service));
}

asio::ip::tcp::socket & game_session::socket()
{
	return _socket;
}

game_session::~game_session()
{
}

game_session::game_session(asio::io_service & _io_service) : _socket(_io_service)
{
}
