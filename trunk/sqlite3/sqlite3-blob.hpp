//
// sqlite3-blob.hpp
// ~~~~~~~~
//
// Copyright (c) 2007 Daniel Pfeifer (purpleKarrot at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __SQLITE3_BLOB_HPP__
#define __SQLITE3_BLOB_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../core/soci-backend.h"
#include "sqlite3-session-fwd.hpp"

namespace db {
namespace details {
namespace sqlite3 {

struct blob_backend : details::blob_backend
{
    blob_backend( session_backend & session );

    ~blob_backend();

    virtual std::size_t get_len();
    virtual std::size_t read( std::size_t offset, char *buf, std::size_t toRead );
    virtual std::size_t write( std::size_t offset, char const *buf, std::size_t toWrite );
    virtual std::size_t append( char const *buf, std::size_t toWrite );
    virtual void trim( std::size_t newLen );

    session_backend & session_;

    std::size_t set_data( char const *buf, std::size_t toWrite );

private:
    char *buf_;
    size_t len_;
};

} // namespace sqlite
} // namespace details
} // namespace db

#endif // __SQLITE3_BLOB_HPP__
