#ifndef BOOST_SQL_MYSQL_STATEMENT_HPP
#define BOOST_SQL_MYSQL_STATEMENT_HPP

#include <boost/sql/mysql/connection.hpp>
#include <boost/sql/mysql/parameters.hpp>

#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/as_vector.hpp> 

namespace boost
{
namespace sql
{
namespace mysql
{

template<typename Params, typename Results>
class statement : private parameters<Params>
{
	typedef typename fusion::result_of::as_vector<Params>::type params_type;
	
public:
	statement(connection& conn, const std::string& query)
	{
		impl = mysql_stmt_init(conn.implementation());
		if (!impl)
			throw std::bad_alloc();

		if (mysql_stmt_prepare(impl, query.c_str(), query.length()) )
		{
			throw std::runtime_error(mysql_stmt_error(impl));
		}
	}

	void execute(const params_type& params)
	{
		bind(impl, params);
		
		if (mysql_stmt_execute(impl))
		{
			throw std::runtime_error(mysql_stmt_error(impl));
		}
	}

	bool fetch(Results& results);

private:
	MYSQL_STMT* impl;
};

} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_MYSQL_STATEMENT_HPP*/
