//
// Copyright (C) 2004-2007 Maciej Sobczak
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "transaction.h"
#include "statement.h"
#include "error.h"

using namespace db;

transaction::transaction( session & sql, std::string const & startCommand )
: handled(false), sql_(sql)
{
	db::statement statement( sql_ );
	try
	{
		statement.alloc();
		statement.prepare( startCommand, db::details::eOneTimeQuery );
		statement.define_and_bind();
		statement.execute(true);
	}
	catch(...)
	{
		statement.clean_up();
		throw;
	}
	statement.clean_up();
}

transaction::~transaction()
{
    if( !handled )
    {
        try
        {
            rollback();
        }
        catch( ... ) {}
    }
}

void transaction::commit()
{
    if( handled )
        throw transaction_error();

	db::statement statement( sql_ );
	try
	{
		statement.alloc();
		statement.prepare( "commit", db::details::eOneTimeQuery );
		statement.define_and_bind();
		statement.execute(true);
	}
	catch(...)
	{
		statement.clean_up();
		throw;
	}
	statement.clean_up();
    handled = true;
}

void transaction::rollback()
{
    if( handled )
        throw transaction_error();

	db::statement statement( sql_ );
	try
	{
		statement.alloc();
		statement.prepare( "rollback", db::details::eOneTimeQuery );
		statement.define_and_bind();
		statement.execute(true);
	}
	catch(...)
	{
		statement.clean_up();
		throw;
	}
	statement.clean_up();
    handled = true;
}

