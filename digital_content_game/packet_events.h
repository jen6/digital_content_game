#pragma once
//���� �̺�Ʈ ���� enum

namespace Packet
{
	using UINT = unsigned int;
	enum class PACKET_EVENT : UINT
	{
		CHECK_SESSION, // string session

					   // ����
		SESSION_NO_MATCH,	//�α��� ���н�
		USER_INFO,

		// Ŭ���̾�Ʈ
		REQUEST_ENTER_MAP, // std::wstring Name, 
		REQUEST_EXIT_MAP, // int object_idx

						  // ����
		ACCEPT_ENTER,
		ACCEPT_EXIT,
		LOAD_UNIT,	// wstring name, int typenum, bool monster, 
					//int state, int fullhp, int nowhp, float x, float y, bool rotate
		GAME_INFO, // int mapnum, int unitnum
		END_LOAD,

				   // ����
		OBJECT_MOVE, // int object_idx, float x, float y
		OBJECT_STATE, // int object_idx, int state
		OBJECT_CHAT, // int object_idx, wstring chat

					 // Ŭ���̾�Ʈ
		OBJECT_ATTACK, // int object_idx, int damage_per

					   // ����
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
		//���� �����̻� �����غ���
	};
}