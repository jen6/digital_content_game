#pragma once
#include <string>
#include <iostream>

#include <boost\lexical_cast.hpp>

#include "../sqlite_modern_cpp.h"

#include "singleton.hpp"
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
		UserDBStruct();
		~UserDBStruct() = default;
		UserDBStruct& operator=(UserDBStruct& user);
		std::string Nickname;	//4
		std::string Skill;		//5
		int Exp;				//6
		int PHp;				//7
		int PAttack;			//8
		int PDefence;			//9
		int Level;				//10
		int Quest;				//11
		std::string UserSession;	//12

		//void Parse(sqxx::statement* st);
		std::string UpdateSql();
	};


	class DbManager : public CppSingleton<DbManager>
	{
	public:

		DbManager();
		~DbManager();

		UserDBStruct GetUser(std::string& session);
		void Update(UserDBStruct& user); 

	private:
		sqlite::database db;
	};
}

