#ifndef __MYSQL_OPTIONS_HPP__
#define __MYSQL_OPTIONS_HPP__

#include <mysql.h>

namespace boost
{
namespace database
{
namespace impl
{
namespace mysql
{

class options
{
public:
	options( const char * db = 0, const char * user = 0, const char * passwd = 0,
			const char * host = 0, unsigned int port = 0, const char * unix_socket = 0,
			unsigned long clientflag = 0 ) :
		db_(db), user_(user), passwd_(passwd), host_(host), port_(port),
		unix_socket_(unix_socket), clientflag_(clientflag)
	{
	}

	options( const options & rhs ) :
		db_(rhs.db_), user_(rhs.user_), passwd_(rhs.passwd_),
		host_(rhs.host_), port_(rhs.port_), unix_socket_(rhs.unix_socket_),
		clientflag_(rhs.clientflag_)
	{		
	}

	const char * db() const
	{
		return db_;
	}

	const char * user() const
	{
		return user_;
	}

	const char * passwd() const
	{
		return passwd_;
	}

	const char * unix_socket() const
	{
		return unix_socket_;
	}

	const char * host() const
	{
		return host_;
	}

	unsigned int port() const
	{
		return port_;
	}

	unsigned long clientflag() const
	{
		return clientflag_;
	}

private:
	const char * db_;
	const char * user_;
	const char * passwd_;
	const char * host_;
	unsigned int port_;
	const char * unix_socket_;
	unsigned long clientflag_;
};

} // end namespace mysql
} // end namespace impl
} // end namespace database
} // end namespace boost

#endif /*__MYSQL_OPTIONS_HPP__*/
