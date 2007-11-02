//
// Copyright (C) 2004-2007 Maciej Sobczak, Stephen Hutton
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define SOCI_SOURCE

#include "error.h"

using namespace db;

const char * transaction_error::what()
{
	return "The transaction object cannot be handled twice.";
}

soci_error::soci_error(std::string const & msg)
     : std::runtime_error(msg)
{
}
