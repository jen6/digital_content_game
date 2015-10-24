#pragma once
#include <string>

#include "../sqxx/sqxx.hpp"

#include "DbSet.h"

namespace DB 
{
	/*
	type User struct {
	Id			int32
	Created			int64
	UserId			string
	UserPw			string
	NickName		string
	Skill			string	//��ų ������ ����
	Exp				int32	//����ġ
	PHp				int32	//�߰��Ȱ͵�
	PAttack			int32
	PDefence		int32
	Level			int32
	Quest			int32	//����Ʈ ���� ��Ȳ
	LoginSession	string
	IsLogin			bool
}
	�α��� ������ ���ǵ� ����
	��ȯ�� �� �ϴ°� �����̴�...
	*/

	
	enum DBINDEX : int { 
		Nickname  = 4, Skill = 5, Exp = 6,
		PHp = 7, PAttack = 8, PDefence = 9, 
		Level = 10, Quest = 11, Session = 12 };

	class UserDBStruct {
	public:
		std::string Nickname;	//4
		std::string Skill;		//5
		int Exp;				//6
		int PHp;				//7
		int PAttack;			//8
		int PDefence;			//9
		int Level;				//10
		int Quest;				//11
		std::string Session;	//12

		void Parse(sqxx::statement st);
	};


	class DbManager
	{
	public:
		DbManager();
		~DbManager();

		UserDBStruct GetUser(std::string& session);
		void Update(const UserDBStruct& user);

	private:
		sqxx::connection dbcon;
	};
}

