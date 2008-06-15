#ifndef BOOST_SQL_SQLITE3_STATEMENT_HPP
#define BOOST_SQL_SQLITE3_STATEMENT_HPP

#include <boost/sql/sqlite3/connection.hpp>
#include <boost/sql/sqlite3/bind_param.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/sql/executable.hpp>
#include <iostream>
#include <string>


namespace boost
{
namespace sql
{
namespace sqlite3
{

template<typename Param = fusion::vector<> >
class statement : public executable<statement<Param>, Param>
{
public:
	statement(connection& c, const std::string& query) :
		stmt(0), conn(c), query_str(query), prepared(false)
	{
	}

	~statement()
	{
		sqlite3_finalize(stmt);
	}

	void prepare()
	{
		if (sqlite3_prepare_v2(conn.implementation(), query_str.c_str(), query_str.size(),
						&stmt, 0) != SQLITE_OK)
		{
			throw std::runtime_error(sqlite3_errmsg(conn.implementation()));
		}

		prepared = true;
	}

	void execute(const Param& params)
	{
		if(!prepared)
			prepare();

		if( sqlite3_reset(stmt) != SQLITE_OK )
			throw std::runtime_error(sqlite3_errmsg(conn.implementation()));

		try
		{
			fusion::for_each( params, bind_param(stmt) );
		}
		catch( int rc )
		{
			throw std::runtime_error(sqlite3_errmsg(conn.implementation()));
		}

		int (*fn)(sqlite3_stmt*) = sqlite3_column_count(stmt) ?
				sqlite3_reset : sqlite3_step;
		int rc = fn(stmt);
		if( rc != SQLITE_DONE && rc != SQLITE_OK )
			throw std::runtime_error(sqlite3_errmsg(conn.implementation()));
	}

private:
	sqlite3_stmt* stmt;
	connection& conn;
	std::string query_str;
	bool prepared;
};

} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_SQLITE3_STATEMENT_HPP*/
