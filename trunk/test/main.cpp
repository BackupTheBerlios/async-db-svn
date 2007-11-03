
#include "soci.h"
#include "../sqlite3/sqlite3.hpp"
#include "sqlite3.hpp"
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <exception>

bool getName( std::string & name )
{
    std::cout << "Enter name: ";
    return std::cin >> name;
}

const char * create = "CREATE TABLE `phonebook` ( `name` varchar(255) NOT NULL default '', "
	"`phone` varchar(255) NOT NULL default '', PRIMARY KEY (`name`) )";

int main()
{
	try
	{
		db::session sql( db::details::sqlite3::singleton, "database" );

		int count;

		db::statement statement( sql );
		try
		{
			statement.alloc();
			statement.prepare( "select count(*) from phonebook", db::details::eOneTimeQuery );
//			statement.prepare( create, db::details::eOneTimeQuery );
			statement.exchange( db::into(count) );
			statement.define_and_bind();
			statement.execute(true);
		}
		catch(...)
		{
			statement.clean_up();
			throw;
		}

		std::cout << "We have " << count << " entries in the phonebook.\n";
/*
		std::string name;
		while (getName(name))
		{
			std::string phone;
			db::eIndicator ind;
			sql << "select phone from phonebook where name = :name",
				db::into(phone, ind), db::use(name);

			if( ind == db::eOK )
			{
				std::cout << "The phone number is " << phone << '\n';
			}
			else
			{
				std::cout << "There is no phone for " << name << '\n';
			}
		}
*/
	}
	catch( const std::exception & e )
	{
		std::cerr << "Error: " << e.what() << '\n';
	}
}

