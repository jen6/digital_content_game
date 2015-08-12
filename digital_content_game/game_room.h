#ifndef _GAME_ROOM
#define _GAME_ROOM

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "game_session.h"

namespace asio =  boost::asio;

//���� �� ������ Ŀ�ؼ��� ���⼭ �ξ��
class game_room
{
public:
	game_room();
	game_room(asio::io_service&);
	~game_room();
private:
	void start_accept();
	asio::ip::tcp::acceptor _acceptor;
};

#endif