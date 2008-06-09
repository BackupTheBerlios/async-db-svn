
#include <boost/sql/postgres.hpp>
#include <exception>
#include <iostream>

namespace db = boost::sql::postgres;

int main()
{
	try
	{
		db::connection conn;

		conn.open("dbname=foo user=postgres password=postgres");

		std::cout << "client version: " << conn.client_version() << std::endl;
		std::cout << "server version: " << conn.server_version() << std::endl;

		conn.execute("DROP TABLE employee");

		conn.execute("CREATE TABLE employee"
				" ( id INT, name CHAR(20), salary INT, PRIMARY KEY (id) )");

		conn.execute("INSERT INTO employee (id, name, salary) "
				"VALUES (1001, 'Thad Beaumont', 44000)");

	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
