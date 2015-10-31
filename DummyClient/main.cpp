#include <iostream>
#include <thread>
#include "client.h"
#include "login.h"

//#define DOLOGINTEST

int main()
{
	//1 로그인 하기
	std::string str = Login::DoLogin(L"googler", L"googleisbest123");
	std::cout << str << std::endl;


	//2 io_service 설정
	boost::asio::io_service io;
	boost::asio::io_service::work work(io);		//io_service가 종료되지 않도록 도와줌
	tcp::resolver resolver(io);
	auto endpoint_iterator = resolver.resolve({ "127.0.0.1","1000" });
	std::thread t1(boost::bind(&boost::asio::io_service::run, &io));

	//3 client객체 만들어 주기
	client cli(io, endpoint_iterator);

	//4 아까 로그인 하고 받은 세션 넘겨주기
	cli.sendSession(Utils::S2Ws(str));

	//5 자유이용
	while (1)
	{
		float x, y;
		std::cin >> x >> y;
		cli.move(x, y);
	}
	t1.join();

}