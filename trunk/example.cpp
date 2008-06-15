#if TEST_MYSQL

# include <boost/sql/mysql.hpp>
# define PARAMS "testdb", "root", "password"
namespace db = boost::sql::mysql;

#elif TEST_POSTGRES

# include <boost/sql/postgres.hpp>
# define PARAMS "dbname=foo user=postgres password=postgres"
namespace db = boost::sql::postgres;
int boost::sql::postgres::stmt_impl_t::counter = 0;

#elif TEST_SQLITE3

# include <boost/sql/sqlite3.hpp>
# define PARAMS "../../../../sqlite3.db"
namespace db = boost::sql::sqlite3;

#endif

#include <exception>
#include <iostream>
#include <boost/fusion/tuple.hpp>

class Employees : public db::connection
{
public:
	Employees() :
#if TEST_POSTGRES
		insert(*this, "INSERT INTO employee (id, name, salary) VALUES ($1, $2, $3)")
#else
		insert(*this, "INSERT INTO employee (id, name, salary) VALUES (?, ?, ?)")
#endif
	{
		open(PARAMS);

		std::cout << "client version: " << client_version() << std::endl;
		std::cout << "server version: " << server_version() << std::endl;
	}

	db::statement< boost::fusion::tuple<int, std::string, int> > insert;
};

int main()
{
	try
	{
		Employees empl;

		empl.execute("DROP TABLE employee");

		empl.execute("CREATE TABLE employee"
			" ( id INT, name CHAR(20), salary INT, PRIMARY KEY (id) )");

		empl.execute("INSERT INTO employee (id, name, salary) "
			"VALUES (1001, 'Thad Beaumont', 44000)");

		empl.insert.execute(boost::fusion::make_tuple(1002, "Horst", 712));
		empl.insert(1003, "Alfred", 7132);

	} catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
