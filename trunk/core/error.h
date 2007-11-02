//
// Copyright (C) 2004-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

#include <stdexcept>
#include <string>

namespace db
{

class transaction_error : public std::exception
{
	virtual const char * what();
};

class soci_error : public std::runtime_error
{
public:
    soci_error(std::string const & msg);
};

} // namespace soci

#endif // ERROR_H_INCLUDED
