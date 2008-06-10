#ifndef BOOST_SQL_POSTGRES_CONNECTION_HPP
#define BOOST_SQL_POSTGRES_CONNECTION_HPP

#include <postgresql/libpq-fe.h>
#include <postgresql/pg_config.h>
#include <boost/shared_ptr.hpp>
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
	connection() :
		impl(0)
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

	void execute(const std::string& cmd)
	{
		boost::shared_ptr<PGresult> res(PQexec(impl, cmd.c_str()), PQclear);
		if (PQresultStatus(res.get()) != PGRES_COMMAND_OK)
		{
			throw std::runtime_error(PQresultErrorMessage(res.get()));
		}
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
