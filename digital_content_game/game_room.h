#pragma once
#include <string>
#include <memory>
#include <thread>
#include <future>
#include <vector>
#include <deque>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "packet.h"
#include "ThreadPool.h"
#include "Logger.h"
#include "dbmanage.h"
#include "utils.h"


class game_room;
class game_session;

using session_ptr = std::shared_ptr<game_session>;
using room_ptr = std::shared_ptr<game_room>;

#include "packet_parser.hpp"
#include "server_logic.h"

namespace asio =  boost::asio;
//http://www.boost.org/doc/libs/1_44_0/doc/html/boost_asio/tutorial/tutdaytime7.html



//각각 유저의 소켓과 결과 보내는 함수가 있음
class game_session 
	: public std::enable_shared_from_this<game_session>
{
public:
	game_room& _game_room;

	static session_ptr create(asio::io_service&, game_room&);
	asio::ip::tcp::socket& socket();
	void start();		//read write 담당하는 부분
	void close();
	void send(Packet::packet_ptr p);		//각 유저에게 전달
	void broadcast(Packet::packet_ptr p);
	Packet::Body_interface* SessionCheck(std::wstring wsession);
	~game_session();
private:
	enum : size_t { MAX_LENGTH = 1024 };

	game_session(asio::io_service&, game_room&);
	void handle_write(const boost::system::error_code& error);
	void handle_read_body(const boost::system::error_code& error);
	void read_header(const boost::system::error_code& error);
	void check_session(const boost::system::error_code& error);
	void handle_check_session(const boost::system::error_code& error);
	
	//void handler(const boost::system::error_code & error, std::size_t recv_size);
	Packet::UserInfoBody * info = nullptr;
	asio::strand strand;
	asio::ip::tcp::socket _socket;
	boost::array<wchar_t, MAX_LENGTH> data_;

};




//게임 방 유저와 커넥션을 여기서 맺어둠
class game_room 
	: public std::enable_shared_from_this<game_room>
{
public:
	//non copyable
	game_room(const game_room &) = delete;
	game_room& operator=(const game_room &) = delete;
	~game_room();
	//일반생성자 복사생성자 복사대입 연산자 삭제

	static room_ptr create(asio::io_service&, unsigned short, ThreadPoolPtr);

	void broadcast(Packet::packet_ptr p); //모든 유저에게 데이터 브로드 케스트
	void PassTask(std::function<void()> task);
	void UserDelete(session_ptr session);
	

	std::mutex mtx; 
	std::string roomnum;
	Logic::MAP map;

	friend class game;
private:
	game_room(asio::io_service&, unsigned short, ThreadPoolPtr);
	void start_accept();
	void handle_accept(session_ptr, const boost::system::error_code&);
	asio::ip::tcp::acceptor _acceptor;
	std::vector<session_ptr> users;	//유저 목록
	std::deque<Packet::packet_ptr> msg_queue;
	ThreadPoolPtr pool;

	
};
