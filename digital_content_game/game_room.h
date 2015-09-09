#ifndef _GAME_ROOM
#define _GAME_ROOM

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <vector>
#include "game_session.h"

namespace asio =  boost::asio;
//http://www.boost.org/doc/libs/1_44_0/doc/html/boost_asio/tutorial/tutdaytime7.html
//게임 방 유저와 커넥션을 여기서 맺어둠
class game_room
{
public:
	game_room(asio::io_service&, unsigned short);
	~game_room();
private:
	void start_accept();
	void handle_accept(session_ptr, const boost::system::error_code&);
	asio::ip::tcp::acceptor _acceptor;
	std::vector<session_ptr> users;	//유저 목록
};

#endif