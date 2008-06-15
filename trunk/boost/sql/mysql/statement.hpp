/**************************************************************
 * Copyright (c) 2008 Daniel Pfeifer                          *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/
#ifndef BOOST_SQL_MYSQL_STATEMENT_HPP
#define BOOST_SQL_MYSQL_STATEMENT_HPP

#include <boost/sql/mysql/connection.hpp>
#include <boost/sql/mysql/parameters.hpp>
#include <boost/sql/executable.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{

template<typename Param>
class statement : public executable<statement<Param>, Param>
{
	parameters<Param> p;

public:
	statement(connection& conn, const std::string& query) :
		query_str(query), prepared(false)
	{
		impl = mysql_stmt_init(conn.implementation());
		if (!impl)
			throw std::bad_alloc();
	}

	~statement()
	{
		mysql_stmt_close(impl);
	}

	void prepare()
	{
		if (mysql_stmt_prepare(impl, query_str.c_str(), query_str.length()))
			throw std::runtime_error(mysql_stmt_error(impl));

		prepared = true;
	}

	void execute(const Param& params)
	{
		if (!prepared)
			prepare();

		p.bind(impl, params);

		if (mysql_stmt_execute(impl))
		{
			throw std::runtime_error(mysql_stmt_error(impl));
		}
	}

private:
	MYSQL_STMT* impl;
	std::string query_str;
	bool prepared;
};

} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_MYSQL_STATEMENT_HPP*/
