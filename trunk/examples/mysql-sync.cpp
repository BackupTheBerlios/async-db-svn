
#include "mysql.hpp"
#include <iostream>

struct insert_stmt
{
	const char * operator()()
	{
		return "INSERT INTO employee (id, name, salary) VALUES (?, ?, ?)";
	}
};

struct select_stmt
{
	const char * operator()()
	{
		return "SELECT name, salary FROM employee";
	}
};

int main()
{
	try
	{
	    boost::asio::io_service io_service;
		
		boost::database::mysql::connection db(io_service);
		
		db.open( "testdb", "root", "korowa" );

		db.exec_once( "DROP TABLE employee" );

		db.exec_once( "CREATE TABLE employee"
				" ( id INT, name CHAR(20), salary INT, PRIMARY KEY (id) ) "
				"ENGINE=InnoDB" );

		db.exec_once(  "INSERT INTO employee (id, name, salary) "
				"VALUES (1001, 'Thad Beaumont', 44000)" );

		int sal = 50000;
		
		db.exec<insert_stmt>( 1002, "Horst Zwiebelhacker", sal );
		db.exec<insert_stmt>( 1003, std::string("Nomi Krughauser"), 70000 );

		db.exec<select_stmt>();
		
		std::string name;
		int salary;
		
		while( db.fetch<select_stmt>(name,salary) )
			std::cout << name << " earns " << salary << " dollar." << std::endl;
	}
	catch( std::exception & e )
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
