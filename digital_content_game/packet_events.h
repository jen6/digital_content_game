#pragma once
//���� �̺�Ʈ ���� enum
enum class PACKET_EVENT : unsigned int
{
	//���� ���� ������ ����
	GAME_START = 0,
	LOAD_MAPINFO,
	LOAD_PLAYER_INFO,
	LOAD_UNIT_INFO,

	OBJECT_STATECH, //������Ʈ ���ݺ���
					//�� �ȿ� �̵� ���� ���� ��ų ��� ���� �ֱ�

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