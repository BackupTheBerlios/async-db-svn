#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <string>
#include <stdexcept>
#include <mysql.h>

#include <boost/shared_ptr.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{

class connection
{
public:
	connection()
	{
		impl = mysql_init(0);
		if ( !impl)
			throw std::bad_alloc();
	}

	~connection()
	{
		mysql_close(impl);
	}

	void open(const char* db=0, const char* user=0, const char* passwd=0,
			const char* host=0, unsigned int port=0, const char* unix_socket=0,
			unsigned long clientflag=0)
	{
		if ( !mysql_real_connect(impl, host, user, passwd, db, port,
				unix_socket, clientflag) )
		{
			throw std::runtime_error(mysql_error(impl));
		}
	}

	unsigned long client_version()
	{
		return mysql_get_client_version();
	}

	unsigned long server_version()
	{
		return mysql_get_server_version(impl);
	}

	void execute(const std::string& query)
	{
		if (mysql_real_query(impl, query.c_str(), query.length()))
		{
			throw std::runtime_error(mysql_error(impl));
		}
		//mysql_field_count()
	}

	MYSQL* implementation() const
	{
		return impl;
	}

private:
	MYSQL* impl;
};

} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /*CONNECTION_H_*/
