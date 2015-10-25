#pragma once
#include <string>

/*
std::string Nickname;	//4
std::string Skill;		//5
int Exp;				//6
int PHp;				//7
int PAttack;			//8
int PDefence;			//9
int Level;				//10
int Quest;				//11
std::string Session;	//12
*/

const std::string DBFILE = "user_db.bin";
const std::string GETUSER_SQL = "select * from UserDb where LoginSession = ";
const std::string UPDATE_SQL = "update UserDb set ";
const std::string TABLENAME = "UserDb.";
//아무래도 이부분을 그냥 만들어주는 함수 하나 작성