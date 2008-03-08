
#include "mysql.hpp"
#include <iostream>

struct drop_table_statement
{
	const char * operator()()
	{
		return "DROP TABLE employee;";
	}
};

struct create_table_statement
{
	const char * operator()()
	{
		return "CREATE TABLE employee"
			" ( id INT, name CHAR(20), salary INT, PRIMARY KEY (id) ) "
			"ENGINE=InnoDB";
	}
};

struct insert_fixed_statement
{
	const char * operator()()
	{
		return "INSERT INTO employee (id, name, salary) "
			"VALUES (1001, 'Thad Beaumont', 44000)";
	}
};

struct insert_named_statement
{
	const char * operator()()
	{
		return "INSERT INTO employee (id, name, salary) "
			"VALUES (:id, :name, :salary)";
	}
};

struct insert_positional_statement
{
	const char * operator()()
	{
		return "INSERT INTO employee (id, name, salary) VALUES (?, ?, ?)";
	}
};

struct select_statement
{
	const char * operator()()
	{
		return "SELECT name, salary FROM employee";
	}
};


void test_funk( int i )
{
	std::cout << i << std::endl;
}

class db_con
{
public:
	db_con( boost::asio::io_service & io_service ) : db(io_service) {}
	~db_con() {}
	
	void start()
	{
		db.async_open( boost::bind(&db_con::open_handler,this,_1,_2),
				"testdb", "root", "korowa" );
	}
	
	void open_handler( int rc, const char * msg )
	{
		if( rc )
		{
			std::cout << msg << std::endl;
			return;
		}
		
		std::cout << "connected" << std::endl;
		
		db.async_exec_once( drop_table_statement(), 
				boost::bind(&db_con::drop_table_handler,this,_1,_2));
	}
	
	void drop_table_handler( int rc, const char * msg )
	{
		if( rc )
			std::cout << msg << std::endl;

		db.async_exec_once( create_table_statement(), 
				boost::bind(&db_con::create_table_handler,this,_1,_2));
	}
	
	void create_table_handler( int rc, const char * msg )
	{
		if( rc )
			std::cout << msg << std::endl;
		
		std::cout << "table created" << std::endl;
		db.async_transaction( boost::bind(&db_con::transaction_handler,this,_1,_2) );
	}
	
	void transaction_handler( int rc, const char * msg )
	{
		if( rc )
			std::cout << msg << std::endl;
		
	}

private:
	boost::database::mysql::connection db;
};

int main()
{
	try
	{
	    boost::asio::io_service io_service;
		
		db_con my_db( io_service );
		
		my_db.start();

	    // Run the io_service until all operations have finished.
	    io_service.run();
	}
	catch( std::exception & e )
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
