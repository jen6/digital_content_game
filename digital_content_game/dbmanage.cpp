#include "dbmanage.h"
#include <thread>
#include <chrono>

namespace DB 
{

	DbManager::DbManager() : db(DBFILE)
	{
		//dbcon.open(DBFILE, sqxx::OPEN_CREATE | sqxx::OPEN_READWRITE);
		//std::cout << dbcon.filename() << std::endl;

	}


	DbManager::~DbManager()
	{
	}

	UserDBStruct DbManager::GetUser(std::string & session)
	{

		//Nickname, Skill, Exp, PHp, PAttack, PDefence, Level, Quest, LoginSession
		std::string sql = GETUSER_SQL + "'" + session + "';";

		UserDBStruct user;
		db << sql.c_str()
			>> [&](std::string NickName, std::string Skill,
				int Exp, int PHp, int PAttack, int PDefence, int Level, int Quest,
				std::string Session)
		{
			std::cout << NickName << " " << Session << std::endl;
			user.Nickname = NickName; user.Skill = Skill;
			user.Exp = Exp; user.PHp = PHp; user.PAttack = PAttack;
			user.PDefence = PDefence; user.Level = Level; user.Quest = Quest;
			user.Session = Session;
		};
		if (session != user.Session)
		{
			throw std::exception("fuck");
		}
		return user;
	}

	void DbManager::Update(UserDBStruct & user)
	{
		std::string sql = user.UpdateSql();
		db << sql;
	}

	UserDBStruct & UserDBStruct::operator=(UserDBStruct & user)
	{
		Nickname = user.Nickname;
		Skill = user.Skill;
		Exp = user.Exp;
		PHp = user.PHp;
		PAttack = user.PAttack;
		PDefence = user.PDefence;
		Level = user.Level;
		Quest = user.Quest;
		Session = user.Session;
		return *this;
	}

	std::string UserDBStruct::UpdateSql()
	{
		std::string ret = UPDATE_SQL;
		ret += TABLENAME + "Nickname" + " = '" + Nickname + "', ";
		ret += TABLENAME + "Skill" + " = '" + Skill + "', ";
		ret += TABLENAME + "Exp" + " = " + boost::lexical_cast<std::string>(Exp) + ", ";
		ret += TABLENAME + "PHp" + " = " + boost::lexical_cast<std::string>(PHp) + ", ";
		ret += TABLENAME + "PAttack" + " = " + boost::lexical_cast<std::string>(PDefence) + ", ";
		ret += TABLENAME + "Level" + " = " + boost::lexical_cast<std::string>(Level) + ", ";
		ret += TABLENAME + "Quest" + " = " + boost::lexical_cast<std::string>(Quest) + ", ";
		ret += TABLENAME + "LoginSession" + " = '" + Session + "' ";
		ret += "where LoginSession = '" + Session + "' ;";
		std::cout << ret << std::endl;
		return ret;
	}
}
