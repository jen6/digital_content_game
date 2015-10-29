#include <iostream>
#include "game_room.h"



room_ptr game_room::create(asio::io_service & io, unsigned short port_num, ThreadPoolPtr tp)
{
	return room_ptr(new game_room(io, port_num, tp));
}

game_room::game_room(asio::io_service &_io_service, unsigned short port_num, ThreadPoolPtr tp)
	: _acceptor(_io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_num)),
	pool(tp)
{
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
		std::cout << "session established!" << std::endl;
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
	for (auto user : users)
	{
		user.get()->send(p);
	}
}

void game_room::PassTask(std::function<Packet::packet_ptr()> task)
{
	auto packet_fut = pool.get()->enqueue(task);
	try {
		packet_fut.wait();
		auto packet = packet_fut.get();
		if (packet)
		{
			broadcast(packet);
		}
		else
		{
			std::cout << "fuck" << std::endl;
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	
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
	asio::async_read(_socket,
		asio::buffer(data_, Packet::HEADER_LEN),
		boost::bind(
			&game_session::read_header,
			this,
			asio::placeholders::error
		)
	);
}

void game_session::read_header(const boost::system::error_code& error)
{
	Packet::Header header = Packet::ParseHeader(data_.data());

	std::cout << "Parsed : " << static_cast<unsigned int>(header.packet_event)<< ", " << header.packet_len << std::endl;

	auto ptr = reinterpret_cast<char*>(data_.data()) + Packet::HEADER_LEN;

	asio::async_read(_socket,
		boost::asio::buffer(ptr, header.packet_len),
		strand.wrap(
			boost::bind(&game_session::handle_read_body,
				this,
				asio::placeholders::error)
			));
}

void game_session::handle_read_body(const boost::system::error_code & error)
{
	static int cnt = 0;
	if (!error)
	{
		cnt += 1;
		std::cout << "read body cnt : " << cnt << std::endl;
		boost::array<wchar_t, 1024> buf(data_);

		auto ptr = std::make_shared<Packet::Packet>(buf);
		std::function<Packet::packet_ptr()> task(
			std::bind(&Packet::Parse<game_room>,
				ptr, _game_room.shared_from_this()
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
		std::cout << "err cnt : " << error.message() << std::endl;
	}
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
		std::cout << "error in write : " << error.message() << " " << error.value() << std::endl;
	}
}

void game_session::broadcast(Packet::packet_ptr p)
{
	_game_room.broadcast(p);
}


//TODO 여기부분 패킷파서 만들기
void game_session::handler(const boost::system::error_code & error, std::size_t recv_size)
{
	static int cnt = 0;
	if (!error)
	{
		cnt += 1;
		std::cout << "recv cnt : " << std::dec << cnt << std::endl;
		boost::array<wchar_t, 1024> buf(data_);
		std::cout << buf.data() << std::endl;

		for (int i = 0; i < recv_size; ++i) {
			std::cout << std::hex << (int)buf.data()[i] << " ";
		}
		std::cout << std::endl;

		auto ptr = std::make_shared<Packet::Packet>(buf);
		std::function<Packet::packet_ptr()> task(
			boost::bind(&Packet::Parse<game_room>,
					ptr, _game_room.shared_from_this()
				)
			);
		_game_room.PassTask(task);
	}
	asio::async_read(_socket, asio::buffer(data_, MAX_LENGTH),
		strand.wrap(boost::bind(
			&game_session::handle_read_body,
			this,
			asio::placeholders::error
			))
		);
}


game_session::~game_session()
{
}

game_session::game_session(asio::io_service & _io_service, game_room& room) : _socket(_io_service), _game_room(room), strand(_io_service)
{
}


