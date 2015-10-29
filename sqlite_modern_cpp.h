#pragma once
#include "sqlite_modern_cpp/utility/function_traits.h"

#include <string.h>
//#include <string>
#include <functional>
#include <stdexcept>
#include <ctime>
#include <tuple>

#include "sqlite3.h"



namespace sqlite {

	struct sqlite_exception : public std::runtime_error {
		sqlite_exception(const char* msg) :runtime_error(msg) {}
	};

	namespace exceptions
	{
		//One more or less trivial derived error class for each SQLITE error.
		//Note the following are not errors so have no classes:
		//SQLITE_OK, SQLITE_NOTICE, SQLITE_WARNING, SQLITE_ROW, SQLITE_DONE
		//
		//Note these names are exact matches to the names of the SQLITE error codes.
		class error : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class internal: public sqlite_exception{ using sqlite_exception::sqlite_exception; };
		class perm : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class abort : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class busy : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class locked : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class nomem : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class readonly : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class interrupt : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class ioerr : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class corrupt : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class notfound : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class full : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class cantopen : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class protocol : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class empty : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class schema : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class toobig : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class constraint : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class mismatch : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class misuse : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class nolfs : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class auth : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class format : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class range : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class notadb : public sqlite_exception { using sqlite_exception::sqlite_exception; };

		//Some additional errors are here for the C++ interface
		class more_rows : public sqlite_exception { using sqlite_exception::sqlite_exception; };
		class no_rows : public sqlite_exception { using sqlite_exception::sqlite_exception; };
	}

	class database;
	class database_binder;

	template<std::size_t> class binder;

	template<typename T> database_binder&& operator <<(database_binder&& db, T const&& val);
	template<typename T> void get_col_from_db(database_binder& db, int inx, T& val);

	class sqlite3_statment
	{
	private:
		sqlite3_stmt* _stmt;

	public:

		sqlite3_stmt** operator&();

		operator sqlite3_stmt*();

		sqlite3_statment(sqlite3_stmt* s);

		~sqlite3_statment();

	};

	template<typename Tuple, int Element = 0, bool Last = (std::tuple_size<Tuple>::value == Element)> struct tuple_iterate
	{
		static void iterate(Tuple& t, database_binder& db)
		{
			get_col_from_db(db, Element, std::get<Element>(t));
			tuple_iterate<Tuple, Element + 1>::iterate(t, db);
		}
	};

	template<typename Tuple, int Element> struct tuple_iterate<Tuple, Element, true>
	{
		static void iterate(Tuple&, database_binder&)
		{
		}
	};

	class database_binder {
	private:
		sqlite3* const _db;
		std::u16string _sql;
		sqlite3_statment _stmt;
		int _inx;

		bool execution_started = false;
		bool throw_exceptions = true;
		bool error_occured = false;

		void _extract(std::function<void(void)> call_back);
		void _extract_single_value(std::function<void(void)> call_back);

		void _prepare();

		template <typename Type>
		using is_sqlite_value = std::integral_constant<
			bool,
			std::is_floating_point<Type>::value
			|| std::is_integral<Type>::value
			|| std::is_same<std::string, Type>::value
			|| std::is_same<std::u16string, Type>::value
			|| std::is_same<sqlite_int64, Type>::value
		>;

		template<typename T> friend database_binder&& operator <<(database_binder&& ddb, T const&& val);
		template<typename T> friend void get_col_from_db(database_binder& ddb, int inx, T& val);

	protected:
		database_binder(sqlite3* db, std::u16string const & sql);

		database_binder(sqlite3* db, std::string const & sql);

	public:
		friend class database;

		~database_binder() noexcept(false);

		void throw_sqlite_error(int error_code);

		void throw_custom_error(const char* str);

		template <typename Result>
		typename std::enable_if<is_sqlite_value<Result>::value, void>::type operator>>(
			Result& value) {
			this->_extract_single_value([&value, this] {
				get_col_from_db(*this, 0, value);
			});
		}



		template<typename... Types>
		void operator>>(std::tuple<Types...>&& values) {
			this->_extract_single_value([&values, this] {
				tuple_iterate<std::tuple<Types...>>::iterate(values, *this);
			});
		}

		template <typename Function>
		typename std::enable_if<!is_sqlite_value<Function>::value, void>::type operator>>(
			Function func) {
			typedef utility::function_traits<Function> traits;

			this->_extract([&func, this]() {
				binder<traits::arity>::run(*this, func);
			});
		}
	};

	class database {
	private:
		sqlite3 * _db;
		bool _connected;
		bool _ownes_db;

	public:
		database(std::u16string & db_name);

		database(std::string const & db_name);

		database(sqlite3* db);

		~database();

		database_binder operator<<(std::string const& sql) const;
		database_binder operator<<(std::u16string const& sql) const;

		operator bool() const;

		sqlite3_int64 last_insert_rowid() const;
	};

	template<std::size_t Count>
	class binder {
	private:
		template <
			typename    Function,
			std::size_t Index
		>
			using nth_argument_type = typename utility::function_traits<
			Function
			>::template argument<Index>;

	public:
		// `Boundary` needs to be defaulted to `Count` so that the `run` function
		// template is not implicitly instantiated on class template instantiation.
		// Look up section 14.7.1 _Implicit instantiation_ of the ISO C++14 Standard
		// and the [dicussion](https://github.com/aminroosta/sqlite_modern_cpp/issues/8)
		// on Github.

		template<
			typename    Function,
			typename... Values,
			std::size_t Boundary = Count
		>
			static typename std::enable_if < (sizeof...(Values) < Boundary), void > ::type run(
				database_binder& db,
				Function&        function,
				Values&&...      values
				) {
			nth_argument_type<Function, sizeof...(Values)> value{};
			get_col_from_db(db, sizeof...(Values), value);

			run<Function>(db, function, std::forward<Values>(values)..., std::move(value));
		}

		template<
			typename    Function,
			typename... Values,
			std::size_t Boundary = Count
		>
			static typename std::enable_if<(sizeof...(Values) == Boundary), void>::type run(
				database_binder&,
				Function&        function,
				Values&&...      values
				) {
			function(std::move(values)...);
		}
	};
}
