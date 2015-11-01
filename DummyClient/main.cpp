#include <iostream>
#include <thread>
#include "client_sock.h"
#include "login.h"

//#define DOLOGINTEST

int main()
{
	//1 로그인 하기
	std::string str = Login::DoLogin(L"admin", L"admin");
	std::cout << str << std::endl;


	//2 io_service 설정
	boost::asio::io_service io;
	boost::asio::io_service::work work(io);		//io_service가 종료되지 않도록 도와줌
	tcp::resolver resolver(io);
	auto endpoint_iterator = resolver.resolve({ "127.0.0.1","1000" });
	std::thread t1(boost::bind(&boost::asio::io_service::run, &io));

	//3 client객체 만들어 주기
	ClientSock cli(io, endpoint_iterator);

	//4 아까 로그인 하고 받은 세션 넘겨주기
	cli.sendSession(Utils::S2Ws(str));

	//5 자유이용

		Packet::EnterMapBody lb;
		lb.hp = 100; lb.att = false; lb.name = "fuck";
		lb.nowhp = 10; lb.rotate = 0; lb.def = 0;
		lb.x = 0; lb.y = 0;
		//lb.Make_packet();
		cli.send(lb.Make_packet());
	t1.join();

}