#include <iostream>
#include <thread>
#include "game_room.h"

int main() {
	boost::asio::io_service _io_service;
	game_room r1(_io_service, 999);
	std::thread t([&]() {_io_service.run(); });
	t.join();
}