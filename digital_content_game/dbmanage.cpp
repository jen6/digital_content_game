#include "dbmanage.h"

namespace DB 
{

	DbManager::DbManager()
	{
		dbcon.open(DBFILE, sqxx::OPEN_READWRITE);

	}


	DbManager::~DbManager()
	{
	}

	UserDBStruct DbManager::GetUser(std::string & session)
	{
		sqxx::statement result = dbcon.run("");

	}

	void UserDBStruct::Parse(sqxx::statement st)
	{
		Nickname	= st.val<std::string>(DBINDEX::Nickname);
		Skill		= st.val<std::string>(DBINDEX::Skill);
		Exp			= st.val<int>(DBINDEX::Exp);
		PHp			= st.val<int>(DBINDEX::PHp);
		PAttack		= st.val<int>(DBINDEX::PAttack);
		PDefence	= st.val<int>(DBINDEX::PDefence);
		Level		= st.val<int>(DBINDEX::Level);
		Quest		= st.val<int>(DBINDEX::Quest);
		Session		= st.val<std::string>(DBINDEX::Session);
	}
}
