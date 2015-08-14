
#ifndef _GAME_SESSION
#define _GAME_SESSION

#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include "test_packet.hpp"

namespace asio = boost::asio;

class game_session;

using session_ptr = std::shared_ptr<game_session>;

//각각 유저의 소켓과 결과 보내는 함수가 있음
class game_session 
	: public std::enable_shared_from_this<game_session>
{
public:
	static session_ptr create(asio::io_service&);
	asio::ip::tcp::socket& socket();
	void start();
	void send();
	~game_session();
private:
	enum : size_t { MAX_LENGTH = 1024 };

	game_session(asio::io_service&);
	void handle_write(const boost::system::error_code& error);
	void handle_read(const boost::system::error_code& error);
	void handler(const boost::system::error_code& error, std::size_t);
	asio::ip::tcp::socket _socket;
	boost::array<char, MAX_LENGTH> data_;
	chat_message msg;
};


#endif

