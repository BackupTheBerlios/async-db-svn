#ifndef BOOST_SQL_POSTGRES_CONNECTION_HPP
#define BOOST_SQL_POSTGRES_CONNECTION_HPP

#include <postgresql/libpq-fe.h>
#include <postgresql/pg_config.h>
#include <stdexcept>

namespace boost
{
namespace sql
{
namespace postgres
{

class connection
{
public:
	connection() : impl(0)
	{
	}

	~connection()
	{
		PQfinish(impl);
	}

	void open(const char* conninfo)
	{
		impl = PQconnectdb(conninfo);
		if (PQstatus(impl) != CONNECTION_OK)
		{
			throw std::runtime_error(PQerrorMessage(impl));
		}
	}

	unsigned long client_version()
	{
		return PG_VERSION_NUM;
	}

	unsigned long server_version()
	{
		return PQserverVersion(impl);
	}

	void execute(const std::string& query)
	{
		PGresult* result = PQexec(impl, query.c_str());
		if( PQresultStatus(result) != PGRES_COMMAND_OK)
		{
			throw std::runtime_error(PQresultErrorMessage(result));
		}
		PQclear(result);
	}

	PGconn* implementation() const
	{
		return impl;
	}

private:
	PGconn* impl;
};

} // end namespace postgres
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_POSTGRES_CONNECTION_HPP*/
