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
		Body_interface * ptr = nullptr, * buf = nullptr;
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
			buf = session.get()->SessionCheck(dynamic_cast<SessionBody*>(ptr)->UserSession);
			if (buf != nullptr)//세션이 정상적으로 인증됐을때
			{
				ret = buf->Make_packet();
				ret->get_body();
			}
			else
			{
				buf = new InfoBody(PACKET_EVENT::SESSION_NO_MATCH);
				ret = buf->Make_packet();
			}
		}
			session.get()->send(ret);
			Log::Logger::Instance()->L("complete send session");
			break;
		case PACKET_EVENT::REQUEST_ENTER_MAP:
		{
			Log::Logger::Instance()->L("request map");
			std::lock_guard<std::mutex> lock(session.get()->_game_room.mtx);
			ptr = new EnterMapBody();
			for (int i = 0; i < length; i++)
			{
				std::cout << (data_ptr + HEADER_IDX)[i] << " ";
			}
			std::cout << std::endl;

			ptr->Make_Body(data_ptr + HEADER_IDX, length);
			std::cout << length << std::endl;	
			EnterMapBody* body = dynamic_cast<EnterMapBody*>(ptr);
			session.get()->_game_room.map.AddUnit(body->name,
			{ body->hp, body->att, body->def }, body->nowhp, { body->x, body->y },
				(Logic::UNIT_ROTATE)body->rotate);
		}
		Log::Logger::Instance()->L("fuck");
		buf = new InfoBody(PACKET_EVENT::ACCEPT_ENTER);
		session.get()->send(buf->Make_packet());

		session.get()->_game_room.PassTask(boost::bind(
			&Logic::MAP::SendUnitInfo<game_session>,
			&session.get()->_game_room.map, session));
		//userinfo보내는거 쓰레드풀에 넣기
			break;

		case PACKET_EVENT::OBJECT_MOVE:
		{
			std::lock_guard<std::mutex> lock(session.get()->_game_room.mtx);
			ptr = new MoveBody();
			ptr->Make_Body(data_ptr + HEADER_IDX, length);
			ret = ptr->Make_packet();
			MoveBody * body = dynamic_cast<MoveBody *>(ptr);
			session.get()->_game_room.map.UnitMove(body->object_idx, 
				Logic::D2D1_POINT_2F(body->x, body->y));
		}
			session.get()->broadcast(ret);
			break;
		default:
			ptr = nullptr;
			std::cout << "no match" << std::endl;
			std::cout << "event : " << event << ",  tester : " << static_cast<UINT>(PACKET_EVENT::TESTER) << std::endl;
			break;
		}
		if (ptr != nullptr)
		{
			delete ptr;
		}
		if (buf != nullptr)
		{
			delete buf;
		}
	}
}