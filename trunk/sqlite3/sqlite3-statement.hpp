//
// sqlite3-factory.hpp
// ~~~~~~~~
//
// Copyright (c) 2007 Daniel Pfeifer (purpleKarrot at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __SQLITE3_STATEMENT_HPP__
#define __SQLITE3_STATEMENT_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "../core/soci-backend.h"
#include "sqlite3-column.hpp"
#include "sqlite3-into.hpp"
#include "sqlite3-into-vector.hpp"
#include "sqlite3-use.hpp"
#include "sqlite3-use-vector.hpp"
#include "sqlite3-session-fwd.hpp"

namespace sqlite_api
{
#include <sqlite3.h>
}

namespace db {
namespace details {
namespace sqlite3 {

struct statement_backend : details::statement_backend
{
    statement_backend( session_backend & session );

    virtual void alloc();
    virtual void clean_up();
    virtual void prepare( std::string const &query, details::eStatementType eType );
    void resetIfNeeded();

    virtual execFetchResult execute( int number );
    virtual execFetchResult fetch( int number );

    virtual int get_number_of_rows();

    virtual std::string rewrite_for_procedure_call( std::string const & query );

    virtual int prepare_for_describe();
    virtual void describe_column( int colNum, eDataType &dtype, std::string &columnName );

    virtual standard_into_type_backend * make_into_type_backend();
    virtual standard_use_type_backend * make_use_type_backend();
    virtual vector_into_type_backend * make_vector_into_type_backend();
    virtual vector_use_type_backend * make_vector_use_type_backend();

    session_backend & session_;
    sqlite_api::sqlite3_stmt * stmt_;
    recordset dataCache_;
    recordset useData_;
    bool databaseReady_;
    bool boundByName_;
    bool boundByPos_;

private:
    execFetchResult loadRS( int totalRows );
    execFetchResult loadOne();
    execFetchResult bindAndExecute( int number );
};

} // namespace sqlite
} // namespace details
} // namespace db

#endif // __SQLITE3_STATEMENT_HPP__
