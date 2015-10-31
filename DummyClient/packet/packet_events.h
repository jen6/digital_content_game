#pragma once
//게임 이벤트 구분 enum

namespace Packet
{
	using UINT = unsigned int;
	enum class PACKET_EVENT : UINT
	{
		GAME_START = 0,

		CHECK_SESSION,		//세션 체크

		PORTAL_USERINFO,	//포탈타고 유저 정보 보낼때

		SESSION_NO_MATCH, //세션 가지고 올바른 유저를 찾지 못했을 경우

		USER_INFO,		//유저 자신에 대한 정보 받기

		LOAD_INFO,		//게임 시작 정보들 관련 정보 로드

		OBJECT_STATECH, //오브젝트 스텟변경
						//이 안에 이동 멈춤 공격 스킬 등등 상태 넣기
		OBJECT_MOVE,

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