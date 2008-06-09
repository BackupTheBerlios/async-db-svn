//#include <windows.h>

#include <boost/sql/mysql/connection.hpp>
#include <boost/sql/mysql/statement.hpp>
#include <iostream>

#include <boost/mpl/vector.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>

namespace db = boost::sql::mysql;

class employees : public db::connection
{
public:
	employees() :
		insert(*this, "INSERT INTO employee (id, name, salary) VALUES (?, ?, ?)")
	{
		open("testdb", "root", "korowa");
	}

	db::statement<
	boost::mpl::vector<int,std::string,int>,
	boost::mpl::vector<>
	> insert;
};

int main()
{
	try
	{
		employees empl;

		//		db::connection conn;
		//		conn.open("testdb", "root", "korowa");

		std::cout << "client version: " << empl.client_version() << std::endl;
		std::cout << "server version: " << empl.server_version() << std::endl;

		empl.execute("DROP TABLE employee");

		empl.execute("CREATE TABLE employee"
				" ( id INT, name CHAR(20), salary INT, PRIMARY KEY (id) ) "
				"ENGINE=InnoDB");

		empl.execute("INSERT INTO employee (id, name, salary) "
				"VALUES (1001, 'Thad Beaumont', 44000)");

		empl.insert.execute( boost::fusion::make_vector(1002, "Nomi Krughauser", 45000) );

	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
}
