#ifndef BOOST_SQL_POSTGRES_STATEMENT_HPP
#define BOOST_SQL_POSTGRES_STATEMENT_HPP

#include <boost/sql/postgres/connection.hpp>
#include <boost/sql/postgres/bind_param.hpp>
#include <boost/sql/basic_statement.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/mpl/for_each.hpp>

namespace boost
{
namespace sql
{
namespace postgres
{

// Generate names for prepared statements
class stmt_impl_t: public std::string
{
public:
	stmt_impl_t() :
		std::string("boost_sql_stmt_" + boost::lexical_cast<std::string>(
				counter++))
	{
	}

private:
	static int counter;
};

template<BOOST_SQL_TEMPLATE_PARAMS>
class statement : public basic_statement<statement<BOOST_SQL_BASE_TEMPL_PARAMS>,BOOST_SQL_BASE_TEMPL_PARAMS>
{
	typedef typename basic_statement<statement, BOOST_SQL_BASE_TEMPL_PARAMS>::param_t param_t;

	enum
	{
		size = mpl::size<param_t>::value
	};

public:
	statement(connection& c, const std::string& query) :
		conn(c), query_str(query)
	{
	}

	~statement()
	{
		try
		{
			conn.execute("deallocate " + stmt);
		}
		catch(...)
		{
	    }
	}

	void prepare()
	{
        Oid types[size];

		mpl::for_each<param_t>(bind_oid(types));

		boost::shared_ptr<PGresult> res(PQprepare(conn.implementation(), stmt.c_str(),
				query_str.c_str(), size, types), PQclear);

		if (!res)
			throw std::bad_alloc();

		if (PQresultStatus(res.get()) != PGRES_COMMAND_OK)
			throw std::runtime_error(PQresultErrorMessage(res.get()));
	}

	void execute(const param_t& params)
	{
		char* values[size];
		int lengths[size];
		int formats[size];

		fusion::for_each(params, bind_param(values,lengths,formats) );

		boost::shared_ptr<PGresult> res(PQexecPrepared(conn.implementation(), stmt.c_str(),
				size, values, lengths, formats, 1), PQclear);

		if (!res)
			throw std::bad_alloc();

		ExecStatusType status = PQresultStatus(res.get());
		if (status != PGRES_TUPLES_OK && status != PGRES_COMMAND_OK)
			throw std::runtime_error(PQresultErrorMessage(res.get()));

	}

private:
	stmt_impl_t stmt;
	connection& conn;
	std::string query_str;
};

} // end namespace postgres
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_POSTGRES_STATEMENT_HPP*/
