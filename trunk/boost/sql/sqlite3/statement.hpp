#ifndef BOOST_SQL_SQLITE3_STATEMENT_HPP
#define BOOST_SQL_SQLITE3_STATEMENT_HPP

#include <boost/sql/sqlite3/connection.hpp>

#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/as_vector.hpp>

#include <boost/mpl/vector.hpp>

#include <iostream>
#include <string>

namespace boost
{
namespace sql
{
namespace sqlite3
{

template<typename Params = mpl::vector<> >
class statement
{
	typedef typename fusion::result_of::as_vector<Params>::type params_type;

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

	void operator()(const params_type& params)
	{
		if( sqlite3_step(stmt) != SQLITE_DONE)
			throw std::runtime_error(sqlite3_errmsg(conn));
	}

private:
	sqlite3_stmt* stmt;
	::sqlite3* conn;
};

} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_SQLITE3_STATEMENT_HPP*/
