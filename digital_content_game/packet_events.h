#pragma once
//게임 이벤트 구분 enum

namespace Packet
{
	using UINT = unsigned int;
	enum class PACKET_EVENT : UINT
	{
		CHECK_SESSION, // string session

					   // 서버
		SESSION_NO_MATCH,	//로그인 실패시
		USER_INFO,

		// 클라이언트
		REQUEST_ENTER_MAP, // std::wstring Name, 
		REQUEST_EXIT_MAP, // int object_idx

						  // 서버
		ACCEPT_ENTER,
		ACCEPT_EXIT,
		LOAD_UNIT,	// wstring name, int typenum, bool monster, 
					//int state, int fullhp, int nowhp, float x, float y, bool rotate
		GAME_INFO, // int mapnum, int unitnum
		END_LOAD,

				   // 공용
		OBJECT_MOVE, // int object_idx, float x, float y
		OBJECT_STATE, // int object_idx, int state
		OBJECT_CHAT, // int object_idx, wstring chat

					 // 클라이언트
		OBJECT_ATTACK, // int object_idx, int damage_per

					   // 서버
		ADD_UNIT,	// wstring name, int typenum, bool monster, 
					//int state, int fullhp, int nowhp, float x, float y, bool rotate
		ERASE_UNIT, // int object_idx
		OBJECT_HIT, // int object_idx, int damage
		GET_EXP, // int exp

		TESTER,
	};

	enum class GAME_INFO : unsigned int
	{
		MAP = 0,
		OBJECT_INFO,
	};

	enum class MAPS : unsigned int
	{
	};

	enum class OBJECT_STATE : unsigned int
	{
		OBJECT_STOP = 0,
		OBJECT_MOVE,
		OBJECT_RUN,
		OBJECT_ATTACK,
		OBJECT_SKILL,
		OBJECT_DEMEGED,
		//추후 상태이상 생각해보기
	};
}