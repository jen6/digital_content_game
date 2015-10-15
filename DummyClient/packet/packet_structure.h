#pragma once
#include <cstdlib>
#include <sstream>
#include <boost/array.hpp>
#include <memory>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include "packet_events.h"

namespace Packet {
	class Packet;
	using packet_ptr = std::shared_ptr<Packet>;

	//�ڷ����� ���ѻ���
	
	enum : unsigned int { HEADER_LEN = 8, MAX_BODY_LEN = 1016 ,MAX_LEN = 1024 };

	class Ticket
	{

	};

	class Packet : public std::enable_shared_from_this<Packet> {
	public:
		Packet() {}
		Packet(const Packet& it) { _data = it._data; _len = it._len; }
		Packet(boost::array<char, 1024>& data) : _data(data) {}
		Packet(PACKET_EVENT pevent, const char * data, UINT len)
		{
			std::memcpy(_data.data(), &pevent, sizeof(UINT));
			std::memcpy(_data.data()+4, &len, sizeof(UINT));
			std::memcpy(_data.data() + HEADER_LEN, data, len);
			_len = HEADER_LEN + len;
		}
		char * data()
		{
			return _data.data();
		}
		const char * data() const
		{
			return _data.data();
		}
		const char * get_body() const
		{
			return _data.data() + HEADER_LEN;
		}

		size_t length() const
		{
			return _len;
		}
		//TODO ���߿� �ʿ��ϸ� ���� �ڵ� �Ẹ�� ���� �ʿ� x
		//void set_size(UINT size)
		//{
		//	_size = size;
		//	if (_size > MAX_BODY_LEN)
		//	{
		//		_size = MAX_BODY_LEN;
		//	}
		//}
	private:
		boost::array<char, MAX_LEN> _data;
		size_t _len = 0;
	};

	//TODO ������ �Ⱦ��� �ſ��ٰ� delete�ٿ�����

	class Header
	{
	public:
		Header() {}
		Header(UINT len, PACKET_EVENT event) : packet_len(len), packet_event(event) {}
		UINT packet_len;	//packet ��ü ����
		PACKET_EVENT packet_event;	//packet evnet����
	};

	class Body_interface 
	{
	public:
		Body_interface() {}
		virtual ~Body_interface() {};
		virtual void Make_Body(const char * packet_body) = 0;
		virtual packet_ptr Make_packet() = 0; //packet �����
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
			std::stringstream ss;
			boost::archive::text_oarchive oa(ss, boost::archive::no_header);
			oa << const_cast<const test &>(*this);
			std::string s = ss.str();
			std::cout << s << std::endl;
			return std::make_shared<Packet>(packet_event, s.c_str(), s.length());
		}

		virtual void Make_Body(const char * packet_body) {
			std::stringstream ss(packet_body);
			boost::archive::text_iarchive ia(ss, boost::archive::no_header);
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
