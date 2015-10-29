#pragma once
#include <iostream>
#include <vector>
#include <thread>

#include "game_room.h"
#include "ThreadPool.h"
#include "dbmanage.h"
#include "Logger.h"

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
	Log::Logger::Instance()->L("ThreadPool Ok");

	//room vector 만들어서 저장하기
	std::vector<room_ptr> rooms;
	for (int i = 0; i < 10; i++) {
		auto room = game_room::create(_io_service,1000+i, tp);
		rooms.push_back(room);
		std::cout << "GameRoom" << i <<" Ok" << std::endl;
	}
	std::thread io_service_thread([&]() { _io_service.run(); });
	std::cout << "io_service_start Ok" << std::endl;
	io_service_thread.join();
}