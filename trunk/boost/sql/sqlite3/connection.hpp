#ifndef BOOST_SQL_SQLITE3_CONNECTION_HPP
#define BOOST_SQL_SQLITE3_CONNECTION_HPP

#include <sqlite3.h>
#include <stdexcept>
#include <exception>

#include <boost/fusion/container/generation/make_vector.hpp>

namespace boost
{
namespace sql
{
namespace sqlite3
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
		sqlite3_close(impl);
	}

	void open(const char* name)
	{
		if (sqlite3_open(name, &impl) != SQLITE_OK)
		{
			throw std::runtime_error(sqlite3_errmsg(impl));
		}
		if (!impl)
		{
			throw std::bad_alloc();
		}
	}

	unsigned long client_version()
	{
		return SQLITE_VERSION_NUMBER;
	}

	unsigned long server_version()
	{
		return sqlite3_libversion_number();
	}

	void execute(const std::string& cmd);

	::sqlite3* implementation() const
	{
		return impl;
	}

private:
	::sqlite3* impl;
};

} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#include <boost/sql/sqlite3/statement.hpp>

namespace boost
{
namespace sql
{
namespace sqlite3
{

void connection::execute(const std::string& cmd)
{
	statement<> stmt(*this, cmd);
	stmt(fusion::make_vector());
}

} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_SQLITE3_CONNECTION_HPP*/
