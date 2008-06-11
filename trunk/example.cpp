#if TEST_MYSQL

# include <boost/sql/mysql.hpp>
# define PARAMS "testdb", "root", "password"
namespace db = boost::sql::mysql;

#elif TEST_POSTGRES

# include <boost/sql/postgres.hpp>
# define PARAMS "dbname=foo user=postgres password=postgres"
namespace db = boost::sql::postgres;

#elif TEST_SQLITE3

# include <boost/sql/sqlite3.hpp>
# define PARAMS "sqlite3.db"
namespace db = boost::sql::sqlite3;

#endif

#include <exception>
#include <iostream>

int main()
{
	try
	{
		db::connection conn;

		conn.open(PARAMS);

		std::cout << "client version: " << conn.client_version() << std::endl;
		std::cout << "server version: " << conn.server_version() << std::endl;

		conn.execute("DROP TABLE employee");

		conn.execute("CREATE TABLE employee"
			" ( id INT, name CHAR(20), salary INT, PRIMARY KEY (id) )");

		conn.execute("INSERT INTO employee (id, name, salary) "
			"VALUES (1001, 'Thad Beaumont', 44000)");

		db::statement<int, std::string, int> stmt(conn,
				"INSERT INTO employee (id, name, salary) VALUES (?, ?, ?)");

		stmt(1002, "Horst", 712);
		stmt(1003, "Alfred", 7132);

	} catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
