#ifndef BOOST_SQL_MYSQL_STATEMENT_HPP
#define BOOST_SQL_MYSQL_STATEMENT_HPP

#include <boost/sql/mysql/connection.hpp>
#include <boost/sql/mysql/parameters.hpp>
#include <boost/sql/basic_statement.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{

template<BOOST_SQL_TEMPLATE_PARAMS>
class statement : public basic_statement<statement<BOOST_SQL_BASE_TEMPL_PARAMS>,BOOST_SQL_BASE_TEMPL_PARAMS>
{
	typedef typename basic_statement<statement, BOOST_SQL_BASE_TEMPL_PARAMS>::param_t param_t;
	parameters<param_t> p;

public:
	statement(connection& conn, const std::string& query) : query_str(query)
	{
		impl = mysql_stmt_init(conn.implementation());
		if (!impl)
			throw std::bad_alloc();
	}

	~statement()
	{
		mysql_stmt_close(impl);
	}

	void prepare()
	{
		if (mysql_stmt_prepare(impl, query_str.c_str(), query_str.length()) )
		{
			throw std::runtime_error(mysql_stmt_error(impl));
		}
	}

	void execute(const param_t& params)
	{
		p.bind(impl, params);

		if (mysql_stmt_execute(impl))
		{
			throw std::runtime_error(mysql_stmt_error(impl));
		}
	}

private:
	MYSQL_STMT* impl;
	std::string query_str;
};

} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_MYSQL_STATEMENT_HPP*/
