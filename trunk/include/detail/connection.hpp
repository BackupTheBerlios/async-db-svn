#ifndef __ASYNC_DB_DETAIL_CONNECTION_HPP__
#define __ASYNC_DB_DETAIL_CONNECTION_HPP__

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/type.hpp>
#include <boost/bind.hpp>
#include <map>

#include <boost/asio.hpp>

namespace boost
{
namespace database
{
namespace detail
{

struct typeinfo
{
	std::type_info const * type;

	explicit typeinfo(std::type_info const & t) :
		type(&t)
	{
	}

	bool operator<(typeinfo const & b) const
	{
		return 0!=(type->before(*b.type));
	}
};

typedef asio::detail::thread thread;

template<typename Service>
class basic_connection : public asio::basic_io_object<Service>
{
public:
	typedef typename Service::statement_impl stmt_type;
	typedef typename Service::options option_type;

	/// Constructor.
	basic_connection(asio::io_service & io_service) :
		asio::basic_io_object<Service>(io_service)
	{
	}

	/// Destructor.
	~basic_connection()
	{
	}

	/// Open a connection to a database.
	template<typename... Args>
	void open( const Args&... args )
	{
		this->service.open( this->implementation, option_type(args...) );
	}

	template <typename Handler, typename... Args>
	void async_open( Handler handler, const Args&... args )
	{
		this->service.async_open( this->implementation, handler, option_type(args...) );
	}

	/// Close the connection.
	void close()
	{
		this->service.close( this->implementation );
	}

	/// Check it the connection is open.
	bool is_open() const
	{
		return this->implementation != this->service.null();
	}

	/// Start a transaction.
	void transaction()
	{
		this->service.transaction( this->implementation );
	}

	template<typename Handler>
	void async_transaction( Handler handler )
	{
		this->service.async_transaction( this->implementation, handler );
	}

	/// Commit a transaction.
	void commit()
	{
		this->service.commit(this->implementation);
	}

	template<typename Handler>
	void async_commit( Handler handler )
	{
		this->service.async_commit( this->implementation, handler );
	}

	/// Rollback a transaction.
	void rollback()
	{
		this->service.rollback(this->implementation);
	}

	template<typename Handler>
	void async_rollback( Handler handler )
	{
		this->service.async_rollback( this->implementation, handler );
	}

	template<typename Statement, typename... Args>
	void exec( Args... args )
	{
		typedef std::pair<typename statement_map::iterator,bool> rv_t;
		rv_t rv = statements.insert(std::make_pair(
				typeinfo(typeid(Statement)), stmt_ptr()) );
		if( rv.second )
		{
			rv.first->second.reset(new stmt_type(this->implementation));
			rv.first->second->prepare( Statement() );
		}
		
		rv.first->second->exec( args... );
	}

	template<std::size_t N>
	void exec_once( const char (&str)[N] )
	{
		this->service.exec_once( this->implementation, str, N );
	}

	void exec_once( const std::string & str )
	{
		this->service.exec_once( this->implementation, str.c_str(), str.size() );
	}

	template<typename Statement, typename Handler>
	void async_exec_once( Statement statement, Handler handler )
	{
		BOOST_MPL_ASSERT_NOT(( is_prepared_statement<Statement> ));
		this->service.async_exec_once( this->implementation, statement, handler );		
	}

	template<typename Statement, typename... Args>
	bool fetch( Args&... args )
	{
		typedef typename statement_map::iterator itr;
		itr res = statements.find(typeinfo(typeid(Statement)) );
		if (res == statements.end() )
			throw std::runtime_error( typeid(Statement).name() );
		return res->second->fetch( args... );
	}

private:
	typedef shared_ptr<stmt_type> stmt_ptr;

	typedef std::map<typeinfo,stmt_ptr> statement_map;
	statement_map statements;
};

} // end namespace detail
} // end namespace database
} // end namespace boost

#endif /*__ASYNC_DB_DETAIL_CONNECTION_HPP__*/
