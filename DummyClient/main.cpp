#include <iostream>
#include <thread>
#include <random>
#include "client.h"


int main()
{
	try {
		boost::asio::io_service io;
		tcp::resolver resolver(io);
		auto endpoint_iterator = resolver.resolve({ "127.0.0.1","999" });

		std::thread t1([&io]() { io.run(); });
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
		t1.join();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}