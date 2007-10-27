//
// basic_database.hpp
// ~~~~~~~~
//
// Copyright (c) 2007 Daniel Pfeifer (purpleKarrot at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __BASIC_DATABASE_HPP__
#define __BASIC_DATABASE_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/basic_io_object.hpp>
#include <string>
#include "database_service.hpp"

namespace boost {
namespace db {

template<typename Backend, typename Service=database_service<Backend> >
class basic_database : public asio::basic_io_object<Service>
{
public:
	explicit basic_database( asio::io_service & io_service )
	  : asio::basic_io_object<Service>( io_service ) {}

	template<typename Table>
	std::size_t query( const std::string & str, Table * table = 0 )
	{
		return this->service.query( this->implementation, str, table );
	}

	template<typename Handler>
	void async_query( const std::string & str, Handler handler )
	{
		this->service.async_query( this->implementation, str, handler );
	}
};

} // namespace db
} // namespace boost

#endif // __BASIC_DATABASE_HPP__

