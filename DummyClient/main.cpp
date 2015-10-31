#include <iostream>
#include <thread>
#include "client.h"
#include "login.h"

//#define DOLOGINTEST

int main()
{
#ifdef DOLOGINTEST
	std::string str = Login::DoLogin(L"googler", L"googleisbest123");
	std::cout << str << std::endl;
#endif

#ifndef DOLOGINTEST
	try {
		boost::asio::io_service io;
		boost::asio::io_service::work work(io);		//io_service가 종료되지 않도록 도와줌
		tcp::resolver resolver(io);
		auto endpoint_iterator = resolver.resolve({ "127.0.0.1","1000" });

		std::thread t1(boost::bind(&boost::asio::io_service::run, &io));
		client cli(io, endpoint_iterator);
		while (1) 
		{
			for (auto i = 0; i < 30; ++i)
			{
				Packet::MoveBody t;
				t.x = i;
				t.y = i;
				auto p = t.Make_packet();
				if (!p) {
					std::cout << "fuck!" << std::endl;
				}
				cli.send(p);
				std::cout << "send cnt = " << std::dec << i << std::endl;
			}
			while (1)
			{
				std::string c;
				std::cin >> c;
				if (c == "end")
				{
					cli.close();
					t1.detach();
					break;
				}

				else if (c == "portal")
				{
					std::cin >> c;
					auto endpoint = resolver.resolve({ "127.0.0.1", c.c_str() });
					cli.reconnect(endpoint);
				}

				else if (c == "test")
				{
					break;
				}

				if (io.stopped())
				{
					std::cout << "io_service stopped" << std::endl;
					break;
				}
			}
			
		}
		if (t1.joinable())
			t1.join();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
#endif
}