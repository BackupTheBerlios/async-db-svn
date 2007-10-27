//
// sqlite3.hpp
// ~~~~~~~~
//
// Copyright (c) 2007 Daniel Pfeifer (purpleKarrot at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __DB_SQLITE3_HPP__
#define __DB_SQLITE3_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace boost {
namespace db {

class sqlite3
{
public:
	typedef basic_database<sqlite3> database;

};

} // namespace db
} // namespace boost

#endif // __DB_SQLITE3_HPP__

