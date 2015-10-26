#include "..\sqlite_modern_cpp.h"

sqlite::sqlite3_statment::operator sqlite3_stmt*()
{
	return _stmt;
}

sqlite::sqlite3_statment::sqlite3_statment(sqlite3_stmt * s)
	:_stmt(s)
{
}

sqlite::sqlite3_statment::~sqlite3_statment()
{
	//Do not check for errors: an error code means that the 
	//*execution* of the statement failed somehow. We deal with errors
	//at that point so we don't need to know about errors here.
	//
	//Also, this is an RAII class to make sure we don't leak during exceptions
	//so there's a reasonably chance we're already in an exception here.

	sqlite3_finalize(_stmt);
}

void sqlite::database_binder::_extract(std::function<void(void)> call_back) {
	execution_started = true;
	int hresult;

	while ((hresult = sqlite3_step(_stmt)) == SQLITE_ROW) {
		call_back();
	}

	if (hresult != SQLITE_DONE) {
		throw_sqlite_error(hresult);
	}

	_stmt = nullptr;
}

void sqlite::database_binder::_extract_single_value(std::function<void(void)> call_back) {
	execution_started = true;
	int hresult;

	if ((hresult = sqlite3_step(_stmt)) == SQLITE_ROW) {
		call_back();
	}
	else if (hresult == SQLITE_DONE)
	{
		if (throw_exceptions)
			throw exceptions::no_rows("no rows to extract: exactly 1 row expected");
	}

	if ((hresult = sqlite3_step(_stmt)) == SQLITE_ROW) {
		if (throw_exceptions)
			throw exceptions::more_rows("not all rows extracted");
	}

	if (hresult != SQLITE_DONE) {
		throw_sqlite_error(hresult);
	}

	_stmt = nullptr;
}

void sqlite::database_binder::_prepare() {
	int hresult;
	if ((hresult = sqlite3_prepare16_v2(_db, _sql.data(), -1, &_stmt, nullptr)) != SQLITE_OK) {
		throw_sqlite_error(hresult);
	}
}

sqlite::database_binder::database_binder(sqlite3 * db, std::u16string const & sql) :
	_db(db),
	_sql(sql),
	_stmt(nullptr),
	_inx(1) {
	_prepare();
}

sqlite::database_binder::database_binder(sqlite3 * db, std::string const & sql) :
	database_binder(db, std::u16string(sql.begin(), sql.end())) { }

sqlite::database_binder::~database_binder() noexcept(false) {

	/* Will be executed if no >>op is found, but not if an exception
	is in mud flight */
	if (!execution_started && !std::current_exception()) {
		int hresult;

		while ((hresult = sqlite3_step(_stmt)) == SQLITE_ROW) {}

		if (hresult != SQLITE_DONE) {
			throw_sqlite_error(hresult);
		}
	}
}

void sqlite::database_binder::throw_sqlite_error(int error_code) {
	if (throw_exceptions) {
		if (error_code == SQLITE_ERROR) throw exceptions::error(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_INTERNAL) throw exceptions::internal  (sqlite3_errmsg(_db));
		else if (error_code == SQLITE_PERM) throw exceptions::perm(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_ABORT) throw exceptions::abort(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_BUSY) throw exceptions::busy(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_LOCKED) throw exceptions::locked(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_NOMEM) throw exceptions::nomem(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_READONLY) throw exceptions::readonly(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_INTERRUPT) throw exceptions::interrupt(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_IOERR) throw exceptions::ioerr(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_CORRUPT) throw exceptions::corrupt(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_NOTFOUND) throw exceptions::notfound(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_FULL) throw exceptions::full(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_CANTOPEN) throw exceptions::cantopen(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_PROTOCOL) throw exceptions::protocol(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_EMPTY) throw exceptions::empty(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_SCHEMA) throw exceptions::schema(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_TOOBIG) throw exceptions::toobig(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_CONSTRAINT) throw exceptions::constraint(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_MISMATCH) throw exceptions::mismatch(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_MISUSE) throw exceptions::misuse(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_NOLFS) throw exceptions::nolfs(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_AUTH) throw exceptions::auth(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_FORMAT) throw exceptions::format(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_RANGE) throw exceptions::range(sqlite3_errmsg(_db));
		else if (error_code == SQLITE_NOTADB) throw exceptions::notadb(sqlite3_errmsg(_db));
		else throw sqlite_exception(sqlite3_errmsg(_db));
	}
	error_occured = true;
}

void sqlite::database_binder::throw_custom_error(const char * str) {
	if (throw_exceptions) {
		throw std::runtime_error(str);
	}
	error_occured = true;
}

sqlite::database::database(std::u16string & db_name) :
	_db(nullptr),
	_connected(false),
	_ownes_db(true) {
	_connected = sqlite3_open16(db_name.data(), &_db) == SQLITE_OK;
}

sqlite::database::database(std::string const & db_name) :
	database(std::u16string(db_name.begin(), db_name.end())) { }

sqlite::database::database(sqlite3 * db) :
	_db(db),
	_connected(SQLITE_OK),
	_ownes_db(false) { }

sqlite::database::~database() {
	if (_db && _ownes_db) {
		sqlite3_close_v2(_db);
		_db = nullptr;
	}
}

sqlite3_int64 sqlite::database::last_insert_rowid() const {
	return sqlite3_last_insert_rowid(_db);
}

namespace sqlite {
	sqlite3_stmt** sqlite3_statment::operator&()
	{
		return &_stmt;
	}

	database_binder database::operator<<(std::string const& sql) const {
		return database_binder(_db, sql);
	}
	database_binder database::operator<<(std::u16string const& sql)const {
		return database_binder(_db, sql);
	}

	database::operator bool() const {
		return _connected;
	}


	// int
	template<> database_binder&& operator <<(database_binder&& db, int const&& val) {
		int hresult;
		if ((hresult = sqlite3_bind_int(db._stmt, db._inx, val)) != SQLITE_OK) {
			db.throw_sqlite_error(hresult);
		}
		++db._inx;
		return std::move(db);
	}
	template<> void get_col_from_db(database_binder& db, int inx, int& val) {
		if (sqlite3_column_type(db._stmt, inx) == SQLITE_NULL) {
			val = 0;
		}
		else {
			val = sqlite3_column_int(db._stmt, inx);
		}
	}

	// sqlite_int64
	template<> database_binder&& operator <<(database_binder&& db, sqlite_int64 const&& val) {
		int hresult;
		if ((hresult = sqlite3_bind_int64(db._stmt, db._inx, val)) != SQLITE_OK) {
			db.throw_sqlite_error(hresult);
		}

		++db._inx;
		return std::move(db);
	}
	template<> void get_col_from_db(database_binder& db, int inx, sqlite3_int64& i) {
		if (sqlite3_column_type(db._stmt, inx) == SQLITE_NULL) {
			i = 0;
		}
		else {
			i = sqlite3_column_int64(db._stmt, inx);
		}
	}

	// float
	template<> database_binder&& operator <<(database_binder&& db, float const&& val) {
		int hresult;
		if ((hresult = sqlite3_bind_double(db._stmt, db._inx, double(val))) != SQLITE_OK) {
			db.throw_sqlite_error(hresult);
		}

		++db._inx;
		return std::move(db);
	}
	template<> void get_col_from_db(database_binder& db, int inx, float& f) {
		if (sqlite3_column_type(db._stmt, inx) == SQLITE_NULL) {
			f = 0;
		}
		else {
			f = float(sqlite3_column_double(db._stmt, inx));
		}
	}

	// double
	template<> database_binder&& operator <<(database_binder&& db, double const&& val) {
		int hresult;
		if ((hresult = sqlite3_bind_double(db._stmt, db._inx, val)) != SQLITE_OK) {
			db.throw_sqlite_error(hresult);
		}

		++db._inx;
		return std::move(db);
	}
	template<> void get_col_from_db(database_binder& db, int inx, double& d) {
		if (sqlite3_column_type(db._stmt, inx) == SQLITE_NULL) {
			d = 0;
		}
		else {
			d = sqlite3_column_double(db._stmt, inx);
		}
	}

	// std::string
	template<> void get_col_from_db(database_binder& db, int inx, std::string & s) {
		if (sqlite3_column_type(db._stmt, inx) == SQLITE_NULL) {
			s = std::string();
		}
		else {
			sqlite3_column_bytes(db._stmt, inx);
			s = std::string((char*)sqlite3_column_text(db._stmt, inx));
		}
	}
	template<> database_binder&& operator <<(database_binder&& db, std::string const&& txt) {
		int hresult;
		if ((hresult = sqlite3_bind_text(db._stmt, db._inx, txt.data(), -1, SQLITE_TRANSIENT)) != SQLITE_OK) {
			db.throw_sqlite_error(hresult);
		}

		++db._inx;
		return std::move(db);
	}
	// std::u16string
	template<> void get_col_from_db(database_binder& db, int inx, std::u16string & w) {
		if (sqlite3_column_type(db._stmt, inx) == SQLITE_NULL) {
			w = std::u16string();
		}
		else {
			sqlite3_column_bytes16(db._stmt, inx);
			w = std::u16string((char16_t *)sqlite3_column_text16(db._stmt, inx));
		}
	}
	template<> database_binder&& operator <<(database_binder&& db, std::u16string const&& txt) {
		int hresult;
		if ((hresult = sqlite3_bind_text16(db._stmt, db._inx, txt.data(), -1, SQLITE_TRANSIENT)) != SQLITE_OK) {
			db.throw_sqlite_error(hresult);
		}

		++db._inx;
		return std::move(db);
	}

	/* call the rvalue functions */
	template<typename T> database_binder&& operator <<(database_binder&& db, T const& val) { return std::move(db) << std::move(val); }

	/*special case for string literals*/
	template<std::size_t N> database_binder&& operator <<(database_binder&& db, const char(&STR)[N])
	{
		return std::move(db) << std::string(STR);
	}
	template<std::size_t N> database_binder&& operator <<(database_binder&& db, const char16_t(&STR)[N])
	{
		return std::move(db) << std::u16string(STR);
	}
}

