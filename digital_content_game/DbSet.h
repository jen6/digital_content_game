#pragma once
#include <string>
#include <tchar.h>

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
namespace DB {
	const std::string DBFILE = "E:\\Projects\\digital_content_game\\Debug\\user_db.bin";
	const std::string GETUSER_SQL = "select Nickname, Skill, Exp, PHp, PAttack, PDefence, Level, Quest, LoginSession from UserDb where LoginSession = ";
	const std::string UPDATE_SQL = "update UserDb set ";
	const std::string TABLENAME = "";
	//�ƹ����� �̺κ��� �׳� ������ִ� �Լ� �ϳ� �ۼ�
}