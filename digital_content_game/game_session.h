
#ifndef _GAME_SESSION
#define _GAME_SESSION

#include <string>
#include <memory>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using session_ptr = std::shared_ptr<game_session>;

//각각 유저의 소켓과 결과 보내는 함수가 있음
class game_session 
	: public std::enable_shared_from_this<game_session>
{
public:
	static session_ptr create(asio::io_service&);
	asio::ip::tcp::socket& socket();
	
	~game_session();
private:
	game_session(asio::io_service&);
	asio::ip::tcp::socket _socket;
};

#endif

