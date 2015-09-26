#pragma once
#include <cstdlib>
#include <boost/array.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp> 
#include <boost/serialization/access.hpp>
#include "packet_events.h"


namespace Packet {

	//자료형과 제한사항
	using UINT = unsigned int;
	enum : unsigned int { HEADER_LEN = 8, MAX_BODY_LEN = 1016 ,MAX_LEN = 1024 };

	class Packet {
	public:
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
		void set_size(UINT size)
		{
			_size = size;
			if (_size > MAX_BODY_LEN)
			{
				_size = MAX_BODY_LEN;
			}
		}
	private:
		boost::array<char, MAX_LEN> _data;
		UINT _size = 0;
	};

	class Header 
	{
	public:
		UINT packet_len;	//packet 전체 길이
		PACKET_EVENT pe;	//packet evnet구별
		virtual Packet make_packet() = 0; //packet 만들기
		Header() {}
	protected:
		PACKET_EVENT event = PACKET_EVENT::GAME_START;
	};

	class InfoBody : public Header
	{
	public:
		InfoBody() 
		{
			event = PACKET_EVENT::LOAD_INFO;
		}
		virtual ~InfoBody(){}
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

	class StateBody : public Header
	{
		StateBody()
		{
			event = PACKET_EVENT::OBJECT_STATECH;
		}
		virtual ~StateBody(){}
		OBJECT_STATE state;
		UINT object_idx;
		UINT x, y;
		UINT damage;
		const PACKET_EVENT evnt = PACKET_EVENT::OBJECT_STATECH;

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& state;
			ar& object_idx;
			ar& x, y;
			ar& damage;
		}
	};


}

