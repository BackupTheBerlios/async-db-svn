//
// sqlite3-rowid.hpp
// ~~~~~~~~
//
// Copyright (c) 2007 Daniel Pfeifer (purpleKarrot at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __SQLITE3_ROWID_HPP__
#define __SQLITE3_ROWID_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../core/soci-backend.h"
#include "sqlite3-session-fwd.hpp"

namespace db {
namespace details {
namespace sqlite3 {

struct rowid_backend : details::rowid_backend
{
    rowid_backend( session_backend & session );

    ~rowid_backend();

    unsigned long value_;
};

} // namespace sqlite
} // namespace details
} // namespace db

#endif // __SQLITE3_ROWID_HPP__
