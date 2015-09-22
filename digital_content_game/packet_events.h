#pragma once
//게임 이벤트 구분 enum

namespace Packet
{
	enum class PACKET_EVENT : unsigned int
	{
		GAME_START = 0,

		LOAD_INFO,		//게임 시작 정보들 관련 정보 로드

		OBJECT_STATECH, //오브젝트 스텟변경
						//이 안에 이동 멈춤 공격 스킬 등등 상태 넣기
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