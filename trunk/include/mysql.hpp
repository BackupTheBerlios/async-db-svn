#ifndef __ASYNC_DB_MYSQL_HPP__
#define __ASYNC_DB_MYSQL_HPP__

#include <boost/asio.hpp>
#include <mysql.h>


#include "detail/error.hpp"

namespace boost
{
namespace database
{

class my_error : public basis_error
{
public:
	my_error(MYSQL * impl) :
		basis_error(mysql_errno(impl), mysql_error(impl))
	{
	}

	my_error(MYSQL_STMT * impl) :
		basis_error(mysql_stmt_errno(impl), mysql_stmt_error(impl))
	{
	}
};

}
}

//#include "impl/mysql/connection.hpp"
#include "impl/mysql/statement.hpp"
#include "impl/mysql/options.hpp"

#include "detail/connection.hpp"

#include <string>

namespace boost
{
namespace database
{

class mysql : public asio::detail::service_base<mysql>
{
public:
	/// The type for an implementation of the connection.
	typedef MYSQL* implementation_type;

//	typedef impl::mysql::connection connection_impl;
	typedef impl::mysql::statement statement_impl;

	typedef detail::basic_connection<mysql> connection;

	typedef impl::mysql::options options;

	/// Constructor initializes the library.
	mysql(asio::io_service & io_service) :
		asio::detail::service_base<mysql>(io_service),
		work_io_service_(new asio::io_service),
		work_(new asio::io_service::work(*work_io_service_)),
		work_thread_(0)
	{
		if ( mysql_library_init(0,0,0))
			throw std::runtime_error( "Unable to start server" );
		work_thread_.reset(new asio::detail::thread(
				work_io_service_runner(*work_io_service_)));
	}

	/// Destructor ends the library.
	~mysql()
	{
		mysql_library_end();
	}

	/// Destroy all user-defined handler objects owned by the service.
	void shutdown_service()
	{
		work_.reset();
		if (work_io_service_)
		{
			work_io_service_->stop();
			if (work_thread_)
			{
				work_thread_->join();
				work_thread_.reset();
			}
			work_io_service_.reset();
		}
	}

	/// Return a null connection implementation.
	implementation_type null() const
	{
		return 0;
	}

	/// Create a new connection implementation.
	void construct( implementation_type & impl )
	{
		impl = mysql_init(0);
		if (!impl)
			throw mysql_error(impl);
	}

	/// Destroy a connection implementation.
	void destroy(implementation_type & impl)
	{
		mysql_close(impl);
		impl = null();
	}

	/// Open a connection implementation.
	void open(implementation_type & impl, const options & opt)
	{
		if ( !mysql_real_connect(impl, opt.host(), opt.user(), opt.passwd(),
				opt.db(), opt.port(), opt.unix_socket(), opt.clientflag() ) )
		{
			std::string errmsg(mysql_error(impl) );
			unsigned int errnum(mysql_errno(impl) );
			mysql_close(impl);

			throw basis_error( errnum, errmsg );
		}
	}

	template <typename Handler>
	class handler_base
	{
	protected:
		handler_base( asio::io_service & io_service, Handler handler ) :
			io_service_(io_service), work_(io_service), handler_(handler)
		{
		}

		void post( int errnum, const char * errmsg )
		{
			io_service_.post( bind(handler_,errnum,errmsg) );
		}

	private:
	    asio::io_service & io_service_;
	    asio::io_service::work work_;
		Handler handler_;
	};

	template <typename Handler>
	class open_handler : public handler_base<Handler>
	{
	public:
		open_handler(implementation_type & impl, const options & opt,
				asio::io_service & io_service, Handler handler ) :
					handler_base<Handler>(io_service,handler), impl_(impl), opt_(opt)
		{
		}

		void operator()()
		{
			unsigned int errnum = 0;
			const char * errmsg = 0;

			if ( !mysql_real_connect(impl_, opt_.host(), opt_.user(), opt_.passwd(),
					opt_.db(), opt_.port(), opt_.unix_socket(), opt_.clientflag() ) )
			{
				errnum = mysql_errno(impl_);
				errmsg = mysql_error(impl_);
			}

			this->post( errnum, errmsg );
		}

	private:
		implementation_type & impl_;
		options opt_;
	};

	template <typename Handler>
	void async_open(implementation_type & impl, Handler handler, const options & opt )
	{
		work_io_service_->post(
				open_handler<Handler>(impl, opt, this->get_io_service(), handler) );
	}

	/// Close a connection implementation.
	void close(implementation_type & impl)
	{
		mysql_close(impl);
		impl = null();
	}

	void exec_once( implementation_type & impl, const char * str, std::size_t )
	{
		if( mysql_query(impl,str) )
			throw my_error(impl);
	}

	template <typename Statement, typename Handler>
	class exec_once_handler : public handler_base<Handler>
	{
	public:
		exec_once_handler(implementation_type & impl, Statement statement,
				asio::io_service & io_service, Handler handler ) :
					handler_base<Handler>(io_service,handler),
					impl_(impl), statement_(statement)
		{
		}

		void operator()()
		{
			unsigned int errnum = 0;
			const char * errmsg = 0;

			if( mysql_query(impl_,statement_()) )
			{
				errnum = mysql_errno(impl_);
				errmsg = mysql_error(impl_);
			}

			this->post( errnum, errmsg );
		}

	private:
		implementation_type & impl_;
		Statement statement_;
	};

	template<typename Statement, typename Handler>
	void async_exec_once( implementation_type & impl, Statement statement, Handler handler )
	{
		work_io_service_->post( exec_once_handler<Statement,Handler>(
				impl, statement, this->get_io_service(), handler) );
	}

	/// Start a transaction.
	void transaction(implementation_type & impl)
	{
		exec_once( impl, "START TRANSACTION", 0 );
	}

	/// Commit a transaction.
	void commit(implementation_type & impl)
	{
		exec_once( impl, "COMMIT", 0 );
	}

	/// Rollback a transaction.
	void rollback(implementation_type & impl)
	{
		exec_once( impl, "ROLLBACK", 0 );
	}

	template<typename Handler>
	void async_transaction( implementation_type & impl, Handler handler )
	{
		work_io_service_->post(
				exec_once_handler<start_transaction_statement,Handler>(
				impl, start_transaction_statement(),
				this->get_io_service(), handler) );
	}

	template<typename Handler>
	void async_commit( implementation_type & impl, Handler handler )
	{
		work_io_service_->post(
				exec_once_handler<commit_transaction_statement,Handler>(
				impl, commit_transaction_statement(),
				this->get_io_service(), handler) );
	}

	template<typename Handler>
	void async_rollback( implementation_type & impl, Handler handler )
	{
		work_io_service_->post(
				exec_once_handler<rollback_transaction_statement,Handler>(
				impl, rollback_transaction_statement(),
				this->get_io_service(), handler) );
	}

private:

	struct start_transaction_statement
	{
		const char * operator()()
		{
			return "START TRANSACTION";
		}
	};

	struct commit_transaction_statement
	{
		const char * operator()()
		{
			return "COMMIT";
		}
	};

	struct rollback_transaction_statement
	{
		const char * operator()()
		{
			return "ROLLBACK";
		}
	};

	/// Helper class to run the work io_service in a thread.
	class work_io_service_runner
	{
	public:
		work_io_service_runner(asio::io_service & io_service) :
			io_service_(io_service)
		{
		}
		void operator()()
		{
			mysql_thread_init();
			io_service_.run();
			mysql_thread_end();
		}
	private:
		asio::io_service & io_service_;
	};

	/// Private io_service used for performing asynchronous operations.
	scoped_ptr<asio::io_service> work_io_service_;

	/// Work for the private io_service to perform.
	scoped_ptr<asio::io_service::work> work_;

	/// Thread used for running the work io_service's run loop.
	scoped_ptr<asio::detail::thread> work_thread_;
};

}
}

#endif /*__ASYNC_DB_MYSQL_HPP__*/
