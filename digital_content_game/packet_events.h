#pragma once
//���� �̺�Ʈ ���� enum

namespace Packet
{
	using UINT = unsigned int;
	enum class PACKET_EVENT : UINT
	{
		GAME_START = 0,

		LOAD_INFO,		//���� ���� ������ ���� ���� �ε�

		OBJECT_STATECH, //������Ʈ ���ݺ���
						//�� �ȿ� �̵� ���� ���� ��ų ��� ���� �ֱ�
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