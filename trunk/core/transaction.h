//
// Copyright (C) 2004-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TRANSACTION_H_INCLUDED
#define TRANSACTION_H_INCLUDED

#include "session.h"

#include <string>

namespace db
{

class transaction
{
public:
    transaction(session &sql,
        std::string const & startCommand = "start transaction");

    ~transaction();

    void commit();
    void rollback();

private:
    bool handled;
    session &sql_;
};

} // namespace soci

#endif // TRANSACTION_H_INCLUDED
