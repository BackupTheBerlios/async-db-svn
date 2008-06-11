#ifndef BOOST_SQL_POSTGRES_STATEMENT_HPP
#define BOOST_SQL_POSTGRES_STATEMENT_HPP

#include <boost/sql/postgres/connection.hpp>
#include <boost/sql/basic_statement.hpp>
#include <boost/lexical_cast.hpp>

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
		std::string("boost_sql_stmt_" + boost::lexical_cast<std::string>(counter++))
	{
	}

private:
	static int counter;
};

template<BOOST_SQL_TEMPLATE_PARAMS>
class statement : public basic_statement<statement<BOOST_SQL_BASE_TEMPL_PARAMS>,BOOST_SQL_BASE_TEMPL_PARAMS>
{
	typedef typename basic_statement<statement, BOOST_SQL_BASE_TEMPL_PARAMS>::param_t param_t;

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
		boost::shared_ptr<PGresult> res(PQprepare(conn.implementation(), stmt.c_str(),
				query_str.c_str(), 0 /*nparams*/, 0 /*argoids*/), PQclear);

		if (!res)
			throw std::bad_alloc();

		if (PQresultStatus(res.get()) != PGRES_COMMAND_OK)
			throw std::runtime_error(PQresultErrorMessage(res.get()));
	}

	void execute(const param_t& params)
	{
		boost::shared_ptr<PGresult> res(PQexecPrepared(conn.implementation(), stmt.c_str(),
				0/*nparams*/, 0/*values*/, 0/*lengths*/, 0/*formats*/, 1), PQclear);

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
