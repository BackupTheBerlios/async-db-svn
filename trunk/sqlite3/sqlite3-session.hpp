//
// sqlite3-session.hpp
// ~~~~~~~~~~~~~~~~~~
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __SQLITE3_SESSION_HPP__
#define __SQLITE3_SESSION_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../core/soci-backend.h"
#include "sqlite3-statement.hpp"
#include "sqlite3-rowid.hpp"
#include "sqlite3-blob.hpp"

namespace sqlite_api
{
#include <sqlite3.h>
}

namespace db {
namespace details {
namespace sqlite3 {

struct session_backend : details::session_backend
{
    session_backend( const std::string & connectString );

    ~session_backend();

    void clean_up();

    virtual details::statement_backend * make_statement_backend();
    virtual rowid_backend * make_rowid_backend();
    virtual blob_backend * make_blob_backend();

    sqlite_api::sqlite3 * conn_;
};

} // namespace sqlite
} // namespace details
} // namespace db

#endif // __SQLITE3_SESSION_HPP__
