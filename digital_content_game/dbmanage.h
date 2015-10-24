#pragma once
#include <string>

namespace DB 
{
	/*
	type User struct {
	Id			int32
	Created			int64
	UserId			string
	UserPw			string
	NickName		string
	Skill			string
	Exp				int32
	PHp				int32
	PAttack			int32
	PDefence		int32
	Level			int32
	Quest			int32
	LoginSession		string
	IsLogin			bool
}
	�α��� ������ ���ǵ� ����
	��ȯ�� �� �ϴ°� �����̴�...
	*/

	class UserDBStruct {
	public:
		int Quest;
		int Exp;
		int PHp;
		int PAttack;
		int Level;
		std::string Skill;
		std::string Nickname;
		std::string Session;
	};


	class DbManager
	{
	public:
		DbManager();
		~DbManager();
	private:

	};
}

