//
// Copyright (C) 2004-2006 Maciej Sobczak, Stephen Hutton, David Courtney
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//


#include "sqlite3-session.hpp"
#include <sstream>

namespace db {
namespace details {
namespace sqlite3 {

using namespace sqlite_api;

namespace // anonymous
{

// helper function for hardcoded queries
void hardExec(sqlite_api::sqlite3 *conn, char const *query, char const *errMsg)
{
    char * zErrMsg = 0;
    int res = sqlite3_exec( conn, query, 0, 0, &zErrMsg );
    if (res != SQLITE_OK)
    {
        std::ostringstream ss;
        ss << errMsg << " " << zErrMsg;

        sqlite3_free( zErrMsg );

        throw soci_error( ss.str() );
    }
}

} // namespace anonymous


session_backend::session_backend( std::string const & connectString )
{
    int res;
    res = sqlite3_open( connectString.c_str(), &conn_ );
    if( res != SQLITE_OK )
    {
        const char *zErrMsg = sqlite3_errmsg( conn_ );

        std::ostringstream ss;
        ss << "Cannot establish connection to the database. "
           << zErrMsg;

        throw soci_error( ss.str() );
    }
}

session_backend::~session_backend()
{
    clean_up();
}

void session_backend::clean_up()
{
    sqlite3_close(conn_);
}

details::statement_backend * session_backend::make_statement_backend()
{
    return new statement_backend( *this );
}

rowid_backend * session_backend::make_rowid_backend()
{
    return new rowid_backend( *this );
}

blob_backend * session_backend::make_blob_backend()
{
    return new blob_backend( *this );
}

} // namespace sqlite3
} // namespace details
} // namespace db
