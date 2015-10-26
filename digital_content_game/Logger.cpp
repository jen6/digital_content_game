#include "Logger.h"

namespace Log {

	Log::Log()
	{
		of.open("Log.txt", std::ios::out);
	}


	Log::~Log()
	{
		of.close();
	}
	//일반적인 로그 로그레벨 0
	void Log::L(std::string msg)
	{
		auto p = system_clock::now();
		std::time_t time = system_clock::to_time_t(p);

		std::string log = "Log L0  ";
		log += std::ctime(&time);
		log += " : " + msg;

		std::cout << log.c_str() << std::endl;

		WriteLog(msg);
	}

	//LogLevel 지정 가능한 로거
	void Log::L(std::string msg, int LogLevel)
	{
		auto p = system_clock::now();
		std::time_t time = system_clock::to_time_t(p);
		
		std::string log = "Log L";
		log += boost::lexical_cast<std::string>(LogLevel); log += "  ";
		log += std::ctime(&time);
		log += " : " + msg;

		std::cout << msg.c_str() << std::endl;

		WriteLog(msg);
	}

	//로그 파일 작성
	void Log::WriteLog(const std::string &log)
	{
		std::lock_guard<std::mutex> lck(mtx);
		LogQueue.push_back(log);
		if (LogQueue.size() > QueueMax)
		{
			for (auto msg : LogQueue)
			{
				of << msg;
			}
			LogQueue.clear();
		}
	}

}
