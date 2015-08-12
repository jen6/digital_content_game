#include "game_session.h"




session_ptr game_session::create(asio::io_service& _io_service)
{
	return session_ptr(new game_session(_io_service));
}

game_session::~game_session()
{
}

game_session::game_session(asio::io_service & _io_service) : _socket(_io_service)
{
}
