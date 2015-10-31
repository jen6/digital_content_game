#pragma once
#include <iostream>
#include <cstdlib>
#include <string>

#include <boost\asio.hpp>
#include <boost\array.hpp>

#include "sha256.h"
#include "utils.h"

using boost::asio::ip::tcp;

namespace Login {
	const std::string SERVER_ADDR = "127.0.0.1";
	const std::string SERVER_PORT = "5678";
	const std::string FAIL_MSG = "1";
	const int MAX_BUFFER = 1024;

	
	std::string DoLogin(std::wstring wId, std::wstring wPw);
}