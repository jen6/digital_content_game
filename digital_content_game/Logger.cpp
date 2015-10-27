#include "Logger.h"

namespace Log {

	Logger::Logger()
	{
		of.open("Log.txt", std::ios::out);
	}


	Logger::~Logger()
	{
		//�Ҹ� �ϱ����� �α׿��� �� ���ֱ�
		if (LogQueue.size())
		{
			for (auto msg : LogQueue)
			{
				of << msg;
			}
			LogQueue.clear();
		}
		of.close();
	}
	//�Ϲ����� �α� �α׷��� 0
	void Logger::L(std::string msg)
	{
		auto p = system_clock::now();
		std::time_t time = system_clock::to_time_t(p);
		std::string time_str = std::ctime(&time);;
		time_str[time_str.length() - 1] = ' ';


		std::string log = "Log L0  " + time_str + ": " + msg;

		std::cout << log.c_str() << std::endl;

		WriteLog(msg);
	}

	//LogLevel ���� ������ �ΰ�
	void Logger::L(std::string msg, int LogLevel)
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

	//�α� ���� �ۼ�
	void Logger::WriteLog(const std::string &log)
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
