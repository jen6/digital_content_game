#include "game_room.h"



game_room::game_room(asio::io_service &_io_service, unsigned short port_num)
	: _acceptor(_io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_num))
{
	start_accept();
}


game_room::~game_room()
{
	
}

void game_room::start_accept()
{
	session_ptr ptr = game_session::create(_acceptor.get_io_service());
	_acceptor.async_accept(ptr.get()->socket(),
		boost::bind(&game_room::handle_accept, this, ptr, asio::placeholders::error));
	users.push_back(ptr);
}

void game_room::handle_accept(session_ptr ptr, const boost::system::error_code& error) 
{
	if (!error)
	{
		start_accept();
	}
}
