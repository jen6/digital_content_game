#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>
#include <memory>
#include <thread>
#include <future>
#include <vector>
#include <deque>
#include "ThreadPool.h"
#include "packet.h"

class game_room;
class game_session;

using session_ptr = std::shared_ptr<game_session>;

namespace asio =  boost::asio;
//http://www.boost.org/doc/libs/1_44_0/doc/html/boost_asio/tutorial/tutdaytime7.html



//각각 유저의 소켓과 결과 보내는 함수가 있음
class game_session
	: public std::enable_shared_from_this<game_session>
{
public:
	static session_ptr create(asio::io_service&, game_room&);
	asio::ip::tcp::socket& socket();
	void start();		//read write 담당하는 부분
	void send(Packet::packet_ptr p);		//각 유저에게 전달
	void broadcast(Packet::packet_ptr p);
	~game_session();
private:
	enum : size_t { MAX_LENGTH = 1024 };

	game_session(asio::io_service&, game_room&);
	void handle_write(const boost::system::error_code& error);
	void handle_read(const boost::system::error_code& error);

	//TODO 여기부분 패킷파서 만들기
	void handler(const boost::system::error_code & error, std::size_t recv_size);

	asio::ip::tcp::socket _socket;
	boost::array<char, MAX_LENGTH> data_;
	game_room& _game_room;
};




//게임 방 유저와 커넥션을 여기서 맺어둠
class game_room
{
public:
	game_room() = delete;
	game_room(asio::io_service&, unsigned short, ThreadPoolPtr);
	void broadcast(Packet::packet_ptr p); //모든 유저에게 데이터 브로드 케스트
	void PassTask(std::function<Packet::packet_ptr()> task);
	~game_room();
private:
	void start_accept();
	void handle_accept(session_ptr, const boost::system::error_code&);
	asio::ip::tcp::acceptor _acceptor;
	std::vector<session_ptr> users;	//유저 목록
	std::deque<Packet::packet_ptr> msg_queue;
	ThreadPoolPtr pool;
};
