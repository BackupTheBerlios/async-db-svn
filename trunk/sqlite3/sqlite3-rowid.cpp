//
// Copyright (C) 2004-2006 Maciej Sobczak, Stephen Hutton, David Courtney
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "sqlite3-rowid.hpp"

namespace db {
namespace details {
namespace sqlite3 {

rowid_backend::rowid_backend( session_backend & /* session */ ) {}

rowid_backend::~rowid_backend() {}

} // namespace sqlite3
} // namespace details
} // namespace db
