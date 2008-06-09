#ifndef BOOST_SQL_POSTGRES_CONNECTION_HPP
#define BOOST_SQL_POSTGRES_CONNECTION_HPP

#include <libpq-fe.h>

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
		if (PQstatus(pgconn) != CONNECTION_OK)
		{
			throw std::runtime_error(PQerrorMessage(impl));
		}
	}

	unsigned long client_version()
	{
		return 0;//mysql_get_client_version();
	}

	unsigned long server_version()
	{
		return 0;//mysql_get_server_version(impl);
	}

	void execute(const std::string& query)
	{
//		if (mysql_real_query(impl, query.c_str(), query.length()))
//		{
//			throw std::runtime_error(mysql_error(impl));
//		}
//		//mysql_field_count()
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
