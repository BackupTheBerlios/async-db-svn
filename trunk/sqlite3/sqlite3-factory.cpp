//
// Copyright (C) 2004-2006 Maciej Sobczak, Stephen Hutton, David Courtney
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "sqlite3-factory.hpp"

db::details::sqlite3::session_backend * db::details::sqlite3::backend_factory::make_session(
     std::string const &connectString) const
{
     return new db::details::sqlite3::session_backend( connectString );
}

db::details::sqlite3::backend_factory const db::details::sqlite3::singleton;

