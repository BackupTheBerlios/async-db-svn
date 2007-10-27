//
// mysql.hpp
// ~~~~~~~~
//
// Copyright (c) 2007 Daniel Pfeifer (purpleKarrot at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __DB_MYSQL_HPP__
#define __DB_MYSQL_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "basic_database.hpp"
#include <mysql.h>

namespace boost {
namespace db {

class mysql : private boost::noncopyable
{
public:
	typedef basic_database<mysql> database;

	typedef MYSQL * implementation_type;

	mysql()
	{

	}

	~mysql()
	{

	}

	void construct( implementation_type & pimpl ) {}
	void destroy( implementation_type & pimpl ) {}



};

} // namespace db
} // namespace boost

#endif // __DB_MYSQL_HPP__

