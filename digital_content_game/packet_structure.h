#pragma once
#include <cstdlib>
#include <boost/array.hpp>

class Packet{
public:

	void set_body(char * str, size_t len)
	{
		body_size = len;
		try {
			std::memcpy(_data.c_array() + PACKET_HEADER, str, len);
			std::memcpy( _data.c_array(), reinterpret_cast<char*>(&len), 4);
		}
		catch (std::exception& e) {
			throw e;
		}
	}

	const char * get_data()
	{
		return _data.c_array() + packet_len();
	}

	const char * get_data()
	{
		return _data.c_array();
	}

	size_t packet_size()
	{
		return body_size + PACKET_HEADER;
	}
private:

	int packet_len()
	{
		int len = 0;
		try {
			std::memcpy(reinterpret_cast<char*>(&len), _data.c_array(), 4);
		}
		catch (std::exception& e) {
			throw e;
		}
		return len;
	}
	enum : size_t {PACKET_HEADER = 4, MAX_DATA = 1024};
	size_t	body_size;
	boost::array<char, MAX_DATA> _data;
};