
#ifndef _GAME_SESSION
#define _GAME_SESSION

#include <string>
#include <memory>
#include <boost/asio.hpp>

namespace asio = boost::asio;

class game_session;

using session_ptr = std::shared_ptr<game_session>;

//���� ������ ���ϰ� ��� ������ �Լ��� ����
class game_session 
	: public std::enable_shared_from_this<game_session>
{
public:
	game_session();
	static session_ptr create(asio::io_service&);
	asio::ip::tcp::socket& socket();
	
	~game_session();
private:
	game_session(asio::io_service&);
	asio::ip::tcp::socket _socket;
};


#endif

