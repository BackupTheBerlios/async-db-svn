/**************************************************************
 * Copyright (c) 2008 Daniel Pfeifer                          *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/
#ifndef BOOST_SQL_SQLITE3_BIND_PARAM_HPP
#define BOOST_SQL_SQLITE3_BIND_PARAM_HPP

#include <sqlite3.h>
#include <string>

namespace boost
{
namespace sql
{
namespace sqlite3
{

class bind_param
{
public:
	bind_param(sqlite3_stmt* stmt) :
		stmt_(stmt), pos_(1)
	{
	}

	template<typename T>
	void operator()(const T& value) const
	{
		throw_if( bind(value));
		++pos_;
	}

private:
	int bind(int value) const
	{
		return sqlite3_bind_int(stmt_, pos_, value);
	}

	int bind(sqlite_int64 value) const
	{
		return sqlite3_bind_int64(stmt_, pos_, value);
	}

	int bind(double value) const
	{
		return sqlite3_bind_double(stmt_, pos_, value);
	}

	int bind(const std::string& value) const
	{
		return sqlite3_bind_text(stmt_, pos_, value.c_str(), value.length(),
				SQLITE_TRANSIENT);
	}

	void throw_if(int rc) const
	{
		if (rc)
			throw rc;
	}

	sqlite3_stmt* stmt_;
	mutable int pos_;
};

} // end namespace sqlite3
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_SQLITE3_BIND_PARAM_HPP*/
