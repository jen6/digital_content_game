#include <iostream>
#include "game_room.h"



room_ptr game_room::create(asio::io_service & io, unsigned short port_num, ThreadPoolPtr tp)
{
	return room_ptr(new game_room(io, port_num, tp));
}

//TODO : 바꾸기
game_room::game_room(asio::io_service &_io_service, unsigned short port_num, ThreadPoolPtr tp)
	: _acceptor(_io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_num)),
	pool(tp), map(0, 0)
{
	roomnum = boost::lexical_cast<std::string>(port_num);
	start_accept();
}


game_room::~game_room()
{
}

void game_room::start_accept()
{
	session_ptr ptr = game_session::create(_acceptor.get_io_service(), *this);
	_acceptor.async_accept(ptr.get()->socket(),
		boost::bind(&game_room::handle_accept, this, ptr, asio::placeholders::error));
}

void game_room::handle_accept(session_ptr ptr, const boost::system::error_code& error) 
{
	if (!error)
	{
		std::lock_guard<std::mutex> lock(mtx);
		users.push_back(ptr);
		ptr.get()->start();
		start_accept();
	}
	else {
		ptr.reset();
	}
}

void game_room::broadcast(Packet::packet_ptr p)
{
	std::lock_guard<std::mutex> lock(mtx);
	for (auto user : users)
	{
		user.get()->send(p);
	}
}

void game_room::PassTask(std::function<void()> task)
{
	auto packet_fut = pool.get()->enqueue(task);
	try {
		packet_fut.get();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

void game_room::UserDelete(session_ptr session)
{
	auto target = std::find(users.begin(), users.end(), session);
	auto ptr = *target;
	users.erase(target);
	


	std::string log = "ptr cnt : " + ptr.use_count();
	Log::Logger::Instance()->L(log + " in room " + roomnum);
	//game 로직부분에서도 삭제하는 부분 필요
}

session_ptr game_session::create(asio::io_service& _io_service, game_room& room)
{
	return session_ptr(new game_session(_io_service, room));
}

asio::ip::tcp::socket & game_session::socket()
{
	return _socket;
}

void game_session::start()
{
	std::string str = _socket.remote_endpoint().address().to_string();
	Log::Logger::Instance()->L("client connected session : " + str + " in room " + _game_room.roomnum);
	asio::async_read(_socket,
		asio::buffer(data_, Packet::HEADER_LEN),
		boost::bind(
			&game_session::read_header,
			this,
			asio::placeholders::error
		)
	);
}

void game_session::close()
{
	boost::system::error_code errorcode;

	if (_socket.is_open())
	{
		_socket.close(errorcode);
		if (errorcode)
		{
			Log::Logger::Instance()->L("socket.close error: " + errorcode.message());
		}
		else
		{
			Log::Logger::Instance()->L("socket closed in room " + _game_room.roomnum);
		}
	}
}

void game_session::read_header(const boost::system::error_code& error)
{
	if (!error)
	{
		Packet::Header header = Packet::ParseHeader(data_.data());

		std::cout << "Parsed : " << static_cast<unsigned int>(header.packet_event) << ", " << header.packet_len << std::endl;

		auto ptr = reinterpret_cast<char*>(data_.data()) + Packet::HEADER_LEN;

		asio::async_read(_socket,
			boost::asio::buffer(ptr, header.packet_len),
			strand.wrap(
				boost::bind(&game_session::handle_read_body,
					this,
					asio::placeholders::error)
				));
	}
	else if (error == boost::asio::error::eof)
	{
		std::string str = _socket.remote_endpoint().address().to_string();
		Log::Logger::Instance()->L("client disconnected session : " + str + " in room " + _game_room.roomnum);
		_game_room.UserDelete(this->shared_from_this());
	}
}

void game_session::handle_read_body(const boost::system::error_code & error)
{
	if (!error)
	{
		boost::array<wchar_t, 1024> buf(data_);

		auto ptr = std::make_shared<Packet::Packet>(buf);
		std::function<void()> task(
			std::bind(&Packet::Parse,
				ptr, this->shared_from_this()
				)
			);

		_game_room.PassTask(task);

		asio::async_read(_socket,
			asio::buffer(data_, Packet::HEADER_LEN),
			boost::bind(
				&game_session::read_header,
				this,
				asio::placeholders::error));
	}
	else {
		std::string str = _socket.remote_endpoint().address().to_string();
		Log::Logger::Instance()->L("client error read body session : " + str);
		std::cout << "err cnt : " << error.message() << std::endl;
	}
}

void game_session::check_session(const boost::system::error_code & error)
{
	if (!error)
	{
		Packet::Header header = Packet::ParseHeader(data_.data());

		std::cout << "Parsed : " << static_cast<unsigned int>(header.packet_event) << ", " << header.packet_len << std::endl;

		auto ptr = reinterpret_cast<char*>(data_.data()) + Packet::HEADER_LEN;

		asio::async_read(_socket,
			boost::asio::buffer(ptr, header.packet_len),
			strand.wrap(
				boost::bind(&game_session::handle_read_body,
					this,
					asio::placeholders::error)
				));
	}
	else if (error == boost::asio::error::eof)
	{
		std::string str = _socket.remote_endpoint().address().to_string();
		Log::Logger::Instance()->L("client disconnected session : " + str);
		_game_room.UserDelete(this->shared_from_this());
	}
}

void game_session::handle_check_session(const boost::system::error_code & error)
{
	if (!error)
	{
		boost::array<wchar_t, Packet::MAX_LEN> buf(data_);

		auto ptr = std::make_shared<Packet::Packet>(buf);


		std::function<void()> task(
			std::bind(&Packet::Parse,
				ptr, this->shared_from_this()
				)
			);

		_game_room.PassTask(task);

		asio::async_read(_socket,
			asio::buffer(data_, Packet::HEADER_LEN),
			boost::bind(
				&game_session::read_header,
				this,
				asio::placeholders::error));
	}
	else {
		std::string str = _socket.remote_endpoint().address().to_string();
		Log::Logger::Instance()->L("client error read body session : " + str);
		std::cout << "err cnt : " << error.message() << std::endl;
	}
}

Packet::Body_interface* game_session::SessionCheck(std::wstring wsession)
{
	std::string session = Utils::Ws2S(wsession);
	//string으로 변환

	Log::Logger::Instance()->L("user : " + session + " is try to session check");

	try {
		auto user = DB::DbManager::Instance()->GetUser(session);
		if (user.UserSession == session)
		{
			Log::Logger::Instance()->L("session match : " + session);
			Packet::UserInfoBody *  userBody = new Packet::UserInfoBody(user);
			info = userBody;
			return dynamic_cast<Packet::Body_interface*>(userBody);
		}
		else
		{
			Log::Logger::Instance()->L("session not match");
			_game_room.UserDelete(this->shared_from_this());
			return nullptr;
			//세션 추가된것을 session vector에서 지워줘야함
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "error in check session : " << e.what() << std::endl;
	}
	//세션이 맞을 경우
	
}

void game_session::send(Packet::packet_ptr p)
{
	asio::async_write(
		_socket,
		asio::buffer(p.get()->data(), p.get()->length()),
		strand.wrap(boost::bind(
		&game_session::handle_write,
		this,
		asio::placeholders::error)));
}

void game_session::handle_write(const boost::system::error_code& error)
{
	if (error)
	{
		//std::cout << "error in write : " << error.message() << " " << error.value() << std::endl;
		std::string str = _socket.remote_endpoint().address().to_string();
		Log::Logger::Instance()->L("client error in send session : " + str);
	}
}

void game_session::broadcast(Packet::packet_ptr p)
{
	_game_room.broadcast(p);
}

game_session::~game_session()
{
	if (info != nullptr)
	{
		delete info;
	}
}

game_session::game_session(asio::io_service & _io_service, game_room& room) : _socket(_io_service), _game_room(room), strand(_io_service)
{
}