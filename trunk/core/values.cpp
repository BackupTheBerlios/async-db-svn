//
// Copyright (C) 2004-2007 Maciej Sobczak, Stephen Hutton
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define SOCI_SOURCE
#include "values.h"
#include "row.h"

using namespace db;
using namespace db::details;

eIndicator values::indicator(std::size_t pos) const
{
    if (row_)
    {
        return row_->indicator(pos);
    }
    else
    {
        return *indicators_[pos];
    }
}

eIndicator values::indicator(std::string const &name) const
{
    if (row_)
    {
        return row_->indicator(name);
    }
    else
    {
        std::map<std::string, std::size_t>::const_iterator it = index_.find(name);
        if (it == index_.end())
        {
            std::ostringstream msg;
            msg << "Column '" << name << "' not found";
            throw soci_error(msg.str());
        }
        return *indicators_[it->second];
    }
}
