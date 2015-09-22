#pragma once
#include <cstdlib>
#include <boost/array.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp> 
#include <boost/serialization/access.hpp>
#include "packet_events.h"


namespace Packet {

	//�ڷ����� ���ѻ���
	using UINT = unsigned int;
	enum : unsigned int { HEADER_LEN = 8 ,MAX_LEN = 1024 };

	struct Header 
	{
		UINT packet_len;	//packet ��ü ����
		PACKET_EVENT pe;	//packet evnet����
	};

	struct InfoBody : public Header
	{
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

	struct StateBody : public Header
	{
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
	};
}

