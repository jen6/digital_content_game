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

	//�ڷ����� ���ѻ���
	
	enum : unsigned int { ZERO_BODY = 0 ,WSTR_REALEN=2, HEADER_IDX = 4, HEADER_LEN = 8, MAX_BODY_LEN = 1016 ,MAX_LEN = 1024 };

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
	private:
		boost::array<wchar_t, MAX_LEN> _data;
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
		virtual void Make_Body(const wchar_t * packet_body, UINT len) = 0;
		virtual packet_ptr Make_packet() = 0; //packet �����
	};

	class EnterMapBody : public Body_interface, Header
	{
	public:
		friend class boost::serialization::access;
		/////
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& name
			& hp
			&att
			&def
			&nowhp
			&x
			&y
			&rotate;
		}
		std::string name;
		int hp;
		int att;
		int def;
		int nowhp;
		int x;
		int y;
		int rotate;

		EnterMapBody()
		{
			packet_event = PACKET_EVENT::REQUEST_ENTER_MAP;
		}
		virtual ~EnterMapBody() {}
		/////
		virtual packet_ptr Make_packet()
		{
			std::wstringstream ss;
			boost::archive::text_woarchive oa(ss, boost::archive::no_header);
			oa << const_cast<const EnterMapBody &>(*this);
			std::wstring s = ss.str();
			return std::make_shared<Packet>(packet_event, s.c_str(), s.length());
		}

		virtual void Make_Body(const wchar_t * packet_body, UINT len) {
			boost::array<wchar_t, 1024> buf;
			std::wmemcpy(buf.data(), packet_body, len);
			buf[len] = 0;
			std::wcout << buf.data() << std::endl;
			std::wstringstream ss(buf.data());
			boost::archive::text_wiarchive ia(ss, boost::archive::no_header);
			ia >> *this;
		}
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

	class SessionBody : public Body_interface, Header
	{
	public:
		std::wstring UserSession;

		SessionBody()
		{
			packet_event = PACKET_EVENT::CHECK_SESSION;
		}

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& UserSession;
		}

		virtual packet_ptr Make_packet()
		{
			std::wstringstream ss;
			boost::archive::text_woarchive oa(ss, boost::archive::no_header);
			oa << const_cast<const SessionBody &>(*this);
			std::wstring s = ss.str();
			return std::make_shared<Packet>(packet_event, s.c_str(), s.length());
		}

		virtual void Make_Body(const wchar_t * packet_body, UINT len) {
			boost::array<wchar_t, 1024> buf;
			std::wmemcpy(buf.data(), packet_body, len);
			buf[len] = 0;
			std::wcout << buf.data() << std::endl;

			std::wstringstream ss(buf.data());
			boost::archive::text_wiarchive ia(ss, boost::archive::no_header);
			ia >> *this;
		}
	};


	class MoveBody : public Body_interface, Header
	{
	public:
		UINT object_idx;
		float x, y;

		MoveBody()
		{
			packet_event = PACKET_EVENT::OBJECT_MOVE;
		}
		virtual ~MoveBody() {}

		friend class boost::serialization::access;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& object_idx;
			ar& x;
			ar& y;
		}

		virtual packet_ptr Make_packet()
		{
			std::wstringstream ss;
			boost::archive::text_woarchive oa(ss, boost::archive::no_header);
			oa << const_cast<const MoveBody &>(*this);
			std::wstring s = ss.str();
			return std::make_shared<Packet>(packet_event, s.c_str(), s.length());
		}

		virtual void Make_Body(const wchar_t * packet_body, UINT len) {
			boost::array<wchar_t, 1024> buf;
			std::wmemcpy(buf.data(), packet_body, len);
			buf[len] = 0;
			std::wcout << buf.data() << std::endl;

			std::wstringstream ss(buf.data());
			boost::archive::text_wiarchive ia(ss, boost::archive::no_header);
			ia >> *this;
		}
	};


	class InfoBody : public Body_interface, Header
	{
	public:
		InfoBody()
		{
			packet_event = PACKET_EVENT::SESSION_NO_MATCH;
		}

		virtual ~InfoBody() {}

		virtual packet_ptr Make_packet() override
		{
			return std::make_shared<Packet>(packet_event, L"", 0);
		}

		virtual void Make_Body(const wchar_t * packet_body, UINT len) override
		{
			//body�� �������� ���� �ʿ� x
			return;
		}
	};


	class StateBody : public Body_interface, Header
	{
	public:
		OBJECT_STATE state;
		UINT object_idx;
		float x, y;
		UINT damage;

		StateBody()
		{
			packet_event = PACKET_EVENT::OBJECT_STATE;
		}
		virtual ~StateBody() {}

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

	class UserInfoBody : public Body_interface, Header {
	public:
		int Idx;
		std::wstring Nickname;	//4
		std::wstring Skill;		//5
		int Exp;				//6
		int PHp;				//7
		int PAttack;			//8
		int PDefence;			//9
		int Level;				//10
		int Quest;				//11
		std::wstring Session;	//12

		friend class boost::serialization::access;

		UserInfoBody() {
			packet_event = PACKET_EVENT::USER_INFO;
		}

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& Idx;
			ar& Nickname;
			ar& Skill;
			ar& Exp;
			ar& PHp;
			ar& PAttack;
			ar& PDefence;
			ar& Level;
			ar& Quest;
			ar& Session;
		}
		virtual packet_ptr Make_packet()
		{
			std::wstringstream ss;
			boost::archive::text_woarchive oa(ss, boost::archive::no_header);
			oa << const_cast<const UserInfoBody &>(*this);
			std::wstring s = ss.str();
			return std::make_shared<Packet>(packet_event, s.c_str(), s.length());
		}

		virtual void Make_Body(const wchar_t * packet_body, UINT len) {
			boost::array<wchar_t, 1024> buf;
			std::wmemcpy(buf.data(), packet_body, len);
			buf[len] = 0;
			std::wcout << buf.data() << std::endl;

			std::wstringstream ss(buf.data());
			boost::archive::text_wiarchive ia(ss, boost::archive::no_header);
			ia >> *this;
		}
	};

	class LoadUnitBody : public Body_interface, Header
	{
	public:
		friend class boost::serialization::access;
		/////
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& name;
			ar& object_type;
			ar& ismonster;
			ar& state;
			ar& fullhp;
			ar& nowhp;
			ar& x;
			ar& y;
			ar& rotate;
		}
		std::wstring name;
		int object_type;
		int ismonster;
		int state;
		int fullhp;
		int nowhp;
		int x;
		int y;
		int rotate;

		LoadUnitBody()
		{
			packet_event = PACKET_EVENT::LOAD_UNIT;
		}
		virtual ~LoadUnitBody() {}
		/////
		virtual packet_ptr Make_packet()
		{
			std::wstringstream ss;
			boost::archive::text_woarchive oa(ss, boost::archive::no_header);
			oa << const_cast<const LoadUnitBody&>(*this);
			std::wstring s = ss.str();
			return std::make_shared<Packet>(packet_event, s.c_str(), s.length());
		}

		virtual void Make_Body(const wchar_t * packet_body, UINT len) {
			boost::array<wchar_t, 1024> buf;
			std::wmemcpy(buf.data(), packet_body, len);
			buf[len] = 0;
			std::wcout << buf.data() << std::endl;
			std::wstringstream ss(buf.data());
			boost::archive::text_wiarchive ia(ss, boost::archive::no_header);
			ia >> *this;
		}
	};

}
