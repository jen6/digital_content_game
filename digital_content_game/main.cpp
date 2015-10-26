#pragma once
#include <iostream>
#include <thread>

#include "game_room.h"
#include "ThreadPool.h"
#include "dbmanage.h"

//#define DEBUG_MODE

int main() {
#ifdef DEBUG_MODE
	try {
	DB::DbManager dbmanage;
	

	std::string session = "jeJZgg0Y_wJqqeS7_NmJvtVp2ZKtoqF-991dmM9GFwc=";
	DB::UserDBStruct user = dbmanage.GetUser(session);
	user.Nickname = "fuck";
	user.Level = 1;
	dbmanage.Update(user);

	user = dbmanage.GetUser(session);
	std::cout << user.Nickname << std::endl;

	session = "1";
	user = dbmanage.GetUser(session);
	std::cout << user.Nickname << std::endl;

	} 
	catch (std::exception& err)
	{
		std::cerr << err.what() << std::endl;
	}
#endif

	//threadpool객체 만들기 그리고 game_room 객체 안에 초기화 할때 넣어주기
	size_t thread_pool_num = 3;
	boost::asio::io_service _io_service;
	ThreadPoolPtr tp = std::shared_ptr<ThreadPool>(new ThreadPool(thread_pool_num));
	std::cout << "Thread Pool Ok" << std::endl;
	auto r1 = game_room::create(_io_service, 999, tp);
	std::cout << "GameRoom1 Ok" << std::endl;
	std::thread io_service_thread([&]() { _io_service.run(); });
	std::cout << "io_service_start Ok" << std::endl;
	io_service_thread.join();
}