//
// postgre.hpp
// ~~~~~~~~
//
// Copyright (c) 2007 Daniel Pfeifer (purpleKarrot at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __DB_POSTGRE_HPP__
#define __DB_POSTGRE_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace boost {
namespace db {

class postgre
{
public:
	typedef basic_database<postgre> database;

};

} // namespace db
} // namespace boost

#endif // __DB_POSTGRE_HPP__

