#pragma once
#include <iostream>
#include <fstream>
#include <deque>
#include <chrono>
#include <ctime>
#include <mutex>

#include <boost\lexical_cast.hpp>

namespace Log {
	using namespace std::chrono;

	class Log
	{
	public:
		Log();
		~Log();

		void L(std::string msg);
		void L(std::string msg, int LogLevel);
	private:
		enum : size_t { QueueMax = 10 };
		void WriteLog(const std::string&);
		
	private:
		std::ofstream of;
		std::deque<std::string> LogQueue;
		std::mutex mtx;
	};
}

