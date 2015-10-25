#include "dbmanage.h"
#include <thread>
#include <chrono>

namespace DB 
{

	DbManager::DbManager()
	{
		dbcon.open(DBFILE, sqxx::OPEN_CREATE | sqxx::OPEN_READWRITE);
		//std::cout << dbcon.filename() << std::endl;

	}


	DbManager::~DbManager()
	{
	}

	UserDBStruct DbManager::GetUser(std::string & session)
	{
		std::string sql = GETUSER_SQL + session;
		sqxx::statement result = dbcon.run(sql);
		while (!result.done())
		{
			result.next_row();
		}

		//std::this_thread::sleep_for(std::chrono::milliseconds(50));
		
		UserDBStruct * user = new UserDBStruct();
		try {
			user->Parse(&result);
		}
		catch (std::exception& err)
		{
			delete(user);
			std::cerr << "error in Get User " << err.what() << std::endl;
		}
		return *user;
	}

	void DbManager::Update(UserDBStruct & user)
	{
		std::string sql = user.UpdateSql();
		sqxx::statement state = dbcon.run(sql);
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

	void UserDBStruct::Parse(sqxx::statement* st)
	{
		std::cout << "Id : " << st->val<int>(0) << std::endl;
		Nickname	= st->val<std::string>(DBINDEX::Nickname);
		Skill		= st->val<std::string>(DBINDEX::Skill);
		Exp			= st->val<int>(DBINDEX::Exp);
		PHp			= st->val<int>(DBINDEX::PHp);
		PAttack		= st->val<int>(DBINDEX::PAttack);
		PDefence	= st->val<int>(DBINDEX::PDefence);
		Level		= st->val<int>(DBINDEX::Level);
		Quest		= st->val<int>(DBINDEX::Quest);
		Session		= st->val<std::string>(DBINDEX::Session);
	}
	std::string UserDBStruct::UpdateSql()
	{
		std::string ret = UPDATE_SQL;
		ret += TABLENAME + "Nickname" + " = " + Nickname + ", ";
		ret += TABLENAME + "Skill" + " = " + Skill + ", ";
		ret += TABLENAME + "Exp" + " = " + boost::lexical_cast<std::string>(Exp) + ", ";
		ret += TABLENAME + "PHp" + " = " + boost::lexical_cast<std::string>(PHp) + ", ";
		ret += TABLENAME + "PAttack" + " = " + boost::lexical_cast<std::string>(PDefence) + ", ";
		ret += TABLENAME + "Level" + " = " + boost::lexical_cast<std::string>(Level) + ", ";
		ret += TABLENAME + "Quest" + " = " + boost::lexical_cast<std::string>(Quest) + ", ";
		ret += TABLENAME + "Session" + " = " + Session + " ";
		ret += "where LoginSession = " + Session;
		return ret;
	}
}
