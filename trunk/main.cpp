

#include "mysql.hpp"
#include <boost/asio.hpp>

int main()
{
	boost::asio::io_service io_service;
	boost::db::mysql::database database1( io_service );
	boost::db::mysql::database database2( io_service );


	std::string end;
	std::cin >> end;

	return 0;
}
