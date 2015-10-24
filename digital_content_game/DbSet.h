#pragma once
#include <string>

const std::string DBFILE = "user_db.bin";
const std::string GETUSER_SQL = "select * from UserDb where LoginSession = ?";
const std::string UPDATE_SQL = "update into UserDb ";