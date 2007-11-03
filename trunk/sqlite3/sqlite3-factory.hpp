//
// sqlite3-factory.hpp
// ~~~~~~~~
//
// Copyright (c) 2007 Daniel Pfeifer (purpleKarrot at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __SQLITE3_FACTORY_HPP__
#define __SQLITE3_FACTORY_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../core/soci-backend.h"
#include "sqlite3-session.hpp"

namespace db {
namespace details {
namespace sqlite3 {

struct backend_factory : db::backend_factory
{
    virtual session_backend * make_session(
        std::string const & connectString ) const;
};

extern backend_factory const singleton;

} // namespace sqlite
} // namespace details
} // namespace db

#endif // __SQLITE3_FACTORY_HPP__
