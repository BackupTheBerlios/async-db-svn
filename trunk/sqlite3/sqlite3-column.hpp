//
// sqlite3-column.hpp
// ~~~~~~~~
//
// Copyright (c) 2007 Daniel Pfeifer (purpleKarrot at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __SQLITE3_COLUMN_HPP__
#define __SQLITE3_COLUMN_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <string>
#include <vector>

namespace db {
namespace details {
namespace sqlite3 {

struct column
{
    std::string data_;
    bool isNull_;
    char * blobBuf_;
    std::size_t blobSize_;
};

typedef std::vector<column> row;
typedef std::vector<row> recordset;


} // namespace sqlite
} // namespace details
} // namespace db

#endif // __SQLITE3_COLUMN_HPP__
