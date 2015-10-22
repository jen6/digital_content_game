#include <iostream>
#include <thread>
#include <random>
//#include "client.h"
#include "login.h"

#define DOLOGINTEST

int main()
{
	std::string str = Login::DoLogin("googler", "googleisbest123");
	std::cout << str << std::endl;

#ifndef DOLOGINTEST
	try {
		boost::asio::io_service io;
		boost::asio::io_service::work work(io);		//io_service가 종료되지 않도록 도와줌
		tcp::resolver resolver(io);
		auto endpoint_iterator = resolver.resolve({ "127.0.0.1","999" });

		std::thread t1(boost::bind(&boost::asio::io_service::run, &io));
		client c(io, endpoint_iterator);


		std::random_device rd;

		for (auto i = 0; i < 30; ++i)
		{
			Packet::test t;
			unsigned int buf = rd();
			t.t = buf;
			std::cout << buf << std::endl;
			auto p = t.Make_packet();
			if (!p) {
				std::cout << "fuck!" << std::endl;
			}
			c.send(p);
		}
		while (1)
		{
			std::string c;
			std::cin >> c;
			if (c == "end")
				break;
			if (io.stopped())
			{
				std::cout << "io_service stopped" << std::endl;
				break;
			}
		}
		t1.join();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
#endif
}