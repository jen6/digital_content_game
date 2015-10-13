#pragma once
#include <string>
#include <memory>
#include <thread>
#include <future>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include "packet.h"
#include "game_room.h"


namespace asio = boost::asio;

class game_session;
//class game_room;

using session_ptr = std::shared_ptr<game_session>;

//���� ������ ���ϰ� ��� ������ �Լ��� ����
class game_session 
	: public std::enable_shared_from_this<game_session>
{
public:
	static session_ptr create(asio::io_service&, game_room&);
	asio::ip::tcp::socket& socket();
	void start();		//read write ����ϴ� �κ�
	void send(Packet::packet_ptr p);		//�� �������� ����
	void broadcast(Packet::packet_ptr p);
	~game_session();
private:
	enum : size_t { MAX_LENGTH = 1024 };

	game_session(asio::io_service&, game_room&);
	void handle_write(const boost::system::error_code& error);
	void handle_read(const boost::system::error_code& error);

	//TODO ����κ� ��Ŷ�ļ� �����
	void handler(const boost::system::error_code & error, std::size_t recv_size);

	asio::ip::tcp::socket _socket;
	boost::array<char, MAX_LENGTH> data_;
	game_room& _game_room;
};
