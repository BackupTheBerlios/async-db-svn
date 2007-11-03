//
// sqlite3-use.hpp
// ~~~~~~~~
//
// Copyright (c) 2007 Daniel Pfeifer (purpleKarrot at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __SQLITE3_USE_HPP__
#define __SQLITE3_USE_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../core/soci-backend.h"
#include "sqlite3-statement-fwd.hpp"

namespace db {
namespace details {
namespace sqlite3 {

struct standard_use_type_backend : details::standard_use_type_backend
{
    standard_use_type_backend( statement_backend & st )
        : statement_(st), buf_(0) {}

    virtual void bind_by_pos( int &position, void *data, details::eExchangeType type);
    virtual void bind_by_name( std::string const &name,
                            void *data, details::eExchangeType type);

    virtual void pre_use(eIndicator const *ind);
    virtual void post_use(bool gotData, eIndicator *ind);

    virtual void clean_up();

    statement_backend & statement_;

    void *data_;
    details::eExchangeType type_;
    int position_;
    std::string name_;
    char *buf_;
};

} // namespace sqlite
} // namespace details
} // namespace db

#endif // __SQLITE3_USE_HPP__
