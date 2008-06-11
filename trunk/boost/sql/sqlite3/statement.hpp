#ifndef BOOST_SQL_SQLITE3_STATEMENT_HPP
#define BOOST_SQL_SQLITE3_STATEMENT_HPP

#include <boost/sql/sqlite3/connection.hpp>
#include <boost/sql/basic_statement.hpp>

#include <iostream>
#include <string>


namespace boost
{
namespace sql
{
namespace sqlite3
{

template<BOOST_SQL_TEMPLATE_PARAMS>
class statement : public basic_statement<statement<BOOST_SQL_BASE_TEMPL_PARAMS>, BOOST_SQL_BASE_TEMPL_PARAMS>
{
	typedef typename basic_statement<statement, BOOST_SQL_BASE_TEMPL_PARAMS>::param_t param_t;

public:
	statement(connection& c, const std::string& cmd) : conn(c.implementation())
	{
		if (sqlite3_prepare_v2(conn, cmd.c_str(), cmd.size(),
						&stmt, 0) != SQLITE_OK)
		{
			throw std::runtime_error(sqlite3_errmsg(conn));
		}
	}

	~statement()
	{
		sqlite3_finalize(stmt);
	}

	void execute(param_t params)
	{
		if( sqlite3_step(stmt) != SQLITE_DONE)
			throw std::runtime_error(sqlite3_errmsg(conn));

		sqlite3_reset(stmt);
	}

private:
	sqlite3_stmt* stmt;
	::sqlite3* conn;
};

} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_SQLITE3_STATEMENT_HPP*/
