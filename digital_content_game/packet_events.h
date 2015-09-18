#pragma once
//게임 이벤트 구분 enum
enum class PACKET_EVENT : unsigned int
{
	//게임 시작 정보들 관련
	GAME_START = 0,
	LOAD_MAPINFO,
	LOAD_PLAYER_INFO,
	LOAD_UNIT_INFO,

	OBJECT_STATECH, //오브젝트 스텟변경
					//이 안에 이동 멈춤 공격 스킬 등등 상태 넣기

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