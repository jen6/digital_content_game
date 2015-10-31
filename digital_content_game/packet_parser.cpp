#include "packet_parser.hpp"
namespace Packet 
{
	Header ParseHeader(wchar_t * ptr)
	{
		UINT length, event;
		wchar_t * data_ptr = ptr;
		auto data = reinterpret_cast<char *>(data_ptr);
		std::memcpy(&event, data, sizeof(UINT));
		std::memcpy(&length,data + 4, sizeof(UINT));
		return Header(length, static_cast<PACKET_EVENT>(event));
	}

	//template<class T>
	//inline packet_ptr Parse(packet_ptr p, std::shared_ptr<T> room)
	void Parse(packet_ptr p, session_ptr session)
	{
		std::cout << "thread id : " << std::this_thread::get_id() << std::endl;
		UINT length, event;
		wchar_t * data_ptr = p.get()->data();
		auto Pdata = reinterpret_cast<char *>(data_ptr);

		std::memcpy(&event, Pdata, sizeof(UINT));
		std::memcpy(&length, Pdata + 4, sizeof(UINT));
		Body_interface * ptr, * userinfobuf;
		packet_ptr ret;

		switch (static_cast<PACKET_EVENT>(event))
		{
		case PACKET_EVENT::TESTER:
		{
			std::lock_guard<std::mutex> lock(session.get()->_game_room.mtx);
			ptr = new test();
			ptr->Make_Body(data_ptr + HEADER_IDX, length);
			ret = game_logic::do_work(std::shared_ptr<test>(dynamic_cast<test*>(ptr)));
		}
			session.get()->broadcast(ret);
			break;

		case PACKET_EVENT::CHECK_SESSION:
		{
			std::lock_guard<std::mutex> lock(session.get()->_game_room.mtx);
			ptr = new SessionBody();
			ptr->Make_Body(data_ptr + HEADER_IDX, length);
			userinfobuf = session.get()->SessionCheck(dynamic_cast<SessionBody*>(ptr)->UserSession);
			if (userinfobuf != nullptr)//세션이 정상적으로 인증됐을때
			{
				dynamic_cast<UserInfoBody*>(userinfobuf)->Idx 
					= session.get()->_game_room.map.GetUnitNum();
				session.get()->_game_room.map.AddUnit(0);
				ret = userinfobuf->Make_packet();
			}
			else
			{
				InfoBody info(PACKET_EVENT::SESSION_NO_MATCH);
				ret = info.Make_packet();
			}
		}
			session.get()->send(ret);
			break;

		case PACKET_EVENT::OBJECT_MOVE:
		{
			std::lock_guard<std::mutex> lock(session.get()->_game_room.mtx);
			ptr = new MoveBody();
			ptr->Make_Body(data_ptr + HEADER_IDX, length);
			ret = ptr->Make_packet();
			MoveBody * body = dynamic_cast<MoveBody *>(ptr);
			session.get()->_game_room.map.MoveUnit(body->object_idx, body->x, body->y);
		}
			session.get()->broadcast(ret);
			break;
		default:
			std::cout << "no match" << std::endl;
			std::cout << "event : " << event << ",  tester : " << static_cast<UINT>(PACKET_EVENT::TESTER) << std::endl;
			break;
		}
	}
}