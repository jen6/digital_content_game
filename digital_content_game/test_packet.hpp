//
// chat_message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef CHAT_MESSAGE_HPP
#define CHAT_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <boost/array.hpp>

class chat_message
{
public:
	enum { header_length = 4 };
	enum { max_body_length = 512 };
	enum : unsigned int { LOGIN = 1, };
	chat_message()
		: body_length_(0)
	{
	}

	//const char* data() const
	//{
	//	return data_;
	//}

	char* data()
	{
		return data_.c_array();
	}

	std::size_t length() const
	{
		return header_length + body_length_;
	}

	//const char* body() const
	//{
	//	return data_.c_array() + header_length;
	//}

	char* body()
	{
		return data_.c_array() + header_length;
	}

	std::size_t body_length() const
	{
		return body_length_;
	}

	void body_length(std::size_t new_length)
	{
		body_length_ = new_length;
		if (body_length_ > max_body_length)
			body_length_ = max_body_length;
	}

	bool decode_header()
	{
		char header[header_length + 1] = "";
		std::strncat(header, data_.c_array(), header_length);
		body_length_ = std::atoi(header);
		if (body_length_ > max_body_length)
		{
			body_length_ = 0;
			return false;
		}
		return true;
	}

	void encode_header()
	{
		char header[header_length + 1] = "";
		std::sprintf(header, "%4d", static_cast<int>(body_length_));
		std::memcpy(data_.c_array(), header, header_length);
	}

private:
	//char data_[header_length + max_body_length];
	boost::array<char, header_length + max_body_length> data_;
	std::size_t body_length_;
};

#endif // CHAT_MESSAGE_HPP


