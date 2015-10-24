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
	Skill			string	//스킬 찍은거 저장
	Exp				int32	//경험치
	PHp				int32	//추가된것들
	PAttack			int32
	PDefence		int32
	Level			int32
	Quest			int32	//퀘스트 진행 상황
	LoginSession	string
	IsLogin			bool
}
	로그인 서버에 정의된 구조
	교환을 잘 하는게 강팀이다...
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

