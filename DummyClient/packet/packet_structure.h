#pragma once
#include <cstdlib>
#include <sstream>
#include <memory>

#include <boost/array.hpp>
#include <boost/archive/text_wiarchive.hpp>
#include <boost/archive/text_woarchive.hpp>

#include <boost/serialization/access.hpp>
#include "packet_events.h"

namespace Packet {
	class Packet;
	using packet_ptr = std::shared_ptr<Packet>;

	//자료형과 제한사항
	
	enum : unsigned int { WSTR_REALEN=2,HEADER_IDX = 4, HEADER_LEN = 8, MAX_BODY_LEN = 1016 ,MAX_LEN = 1024 };

	class Ticket
	{

	};

	class Packet : public std::enable_shared_from_this<Packet> {
	public:
		Packet() {}
		Packet(const Packet& it) { _data = it._data; _len = it._len; }
		Packet(boost::array<wchar_t, 1024>& data) : _data(data) {}
		Packet(PACKET_EVENT pevent, const wchar_t * data, UINT len)
		{
			len *= WSTR_REALEN;
			_len = HEADER_LEN + len;
			auto header_len_ptr = reinterpret_cast<char *>(_data.data()) + 4;

			std::memcpy(_data.data(), &pevent, sizeof(UINT));
			std::memcpy(header_len_ptr, &len, sizeof(UINT));
			std::wmemcpy(_data.data() + HEADER_IDX, data, len);
			
		}
		wchar_t * data()
		{
			return _data.data();
		}
		const wchar_t * data() const
		{
			return _data.data();
		}
		const wchar_t * get_body() const
		{
			return _data.data() + HEADER_IDX;
		}

		size_t length() const
		{
			return _len;
		}
		//TODO 나중에 필요하면 막상 코드 써보니 굳이 필요 x
		//void set_size(UINT size)
		//{
		//	_size = size;
		//	if (_size > MAX_BODY_LEN)
		//	{
		//		_size = MAX_BODY_LEN;
		//	}
		//}
	private:
		boost::array<wchar_t, MAX_LEN> _data;
		size_t _len = 0;
	};

	//TODO 생성자 안쓰는 거에다가 delete붙여놓기

	class Header
	{
	public:
		Header() {}
		Header(UINT len, PACKET_EVENT event) : packet_len(len), packet_event(event) {}
		UINT packet_len;	//packet 전체 길이
		PACKET_EVENT packet_event;	//packet evnet구별
	};

	class Body_interface 
	{
	public:
		Body_interface() {}
		virtual ~Body_interface() {};
		virtual void Make_Body(const wchar_t * packet_body, UINT len) = 0;
		virtual packet_ptr Make_packet() = 0; //packet 만들기
	};

	class test : public Body_interface, Header
	{
	public:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& t;
		}
		UINT t;

		test()
		{
			packet_event = PACKET_EVENT::TESTER;
		}
		virtual ~test() {}

		virtual packet_ptr Make_packet()
		{
			std::wstringstream ss;
			boost::archive::text_woarchive oa(ss, boost::archive::no_header);
			oa << const_cast<const test &>(*this);

			std::wstring data = ss.str();
			std::wcout << data << std::endl;
			return std::make_shared<Packet>(packet_event, data.c_str(), data.length());
		}

		virtual void Make_Body(const wchar_t * packet_body, UINT len) 
		{
			boost::array<wchar_t, 1024> buf;
			std::wmemcpy(buf.data(), packet_body, len);
			
			std::wstringstream ss(buf.data());
			boost::archive::text_wiarchive ia(ss, boost::archive::no_header);
			ia >> *this;
		}
	};

	/*
	class InfoBody : public Body_interface, Header
	{
	public:
		InfoBody() 
		{
			event = PACKET_EVENT::LOAD_INFO;
		}
		
		virtual ~InfoBody(){}

		virtual Packet Make_packet()
		{
			
		}

		GAME_INFO info;
		UINT set;
		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& info;
			ar& set;
		}
	};

	class StateBody : public Body_interface, Header
	{
		StateBody()
		{
			packet_event = PACKET_EVENT::OBJECT_STATECH;
		}
		virtual ~StateBody(){}
		OBJECT_STATE state;
		UINT object_idx;
		UINT x, y;
		UINT damage;

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& state;
			ar& object_idx;
			ar& x, y;
			ar& damage;
		}
	};*/
}
