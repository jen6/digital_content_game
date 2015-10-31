#include "login.h"


namespace Login 
{
	
	std::string DoLogin(std::wstring wId, std::wstring wPw)
	{
		boost::asio::io_service io_service;
		tcp::resolver resolver(io_service);
		auto endpoint_iterator = resolver.resolve({ SERVER_ADDR, SERVER_PORT });
		tcp::resolver::iterator end;

		tcp::socket socket(io_service);

		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end)
		{
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}

		// 접속 실패인지 확인 
		if (error)
			return FAIL_MSG;

		std::string Id = Utils::Ws2S(wId);
		std::string Pw = Utils::Ws2S(wPw);
		//string으로 변환

		//서버로 보낼 문자열 조합
		std::string buf = Id + " ";
		buf += cryptlite::sha256::hash_base64(Pw.c_str());
		buf += "\n";

		//문자열 전송
		boost::array<char, MAX_BUFFER> buffer;
		size_t buflen = buf.length();
		std::strncpy(buffer.data(), buf.c_str(), buflen);
		socket.send(boost::asio::buffer(buffer.data(), buflen));

		//버퍼 초기화
		buffer.fill(0);

		//결과값 받기
		auto RecvBuffer = boost::asio::buffer(buffer);
		size_t len = socket.read_some(RecvBuffer, error);
		std::cout << "len : " << len << std::endl;
		return std::string(buffer.data(), len);
	}
}
