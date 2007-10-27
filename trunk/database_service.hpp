//
// database_service.hpp
// ~~~~~~~~
//
// Copyright (c) 2007 Daniel Pfeifer (purpleKarrot at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __DATABASE_SERVICE_HPP__
#define __DATABASE_SERVICE_HPP__

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/basic_io_object.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <string>

namespace boost {
namespace db {

template<typename Backend>
class database_service : public boost::asio::io_service::service
{
public:
	typedef typename Backend::implementation_type implementation_type;

	/// Constructor creates a thread to run a private io_service.
	database_service( asio::io_service& io_service )
	  : asio::io_service::service( io_service ),
	    work_io_service(),
	    work( new boost::asio::io_service::work(work_io_service) ),
	    work_thread( new boost::thread(
          boost::bind(&boost::asio::io_service::run, &work_io_service)) )
    {
    }

	/// Destructor shuts down the private io_service.
	~database_service()
	{
		/// Indicate that we have finished with the private io_service. Its
	    /// io_service::run() function will exit once all other work has completed.
	    work.reset();
	    if( work_thread )
	    	work_thread->join();
	}

	void shutdown_service()
	{
	}

	void construct( implementation_type & impl ) {}

	void destroy( implementation_type & impl ) {}

	template<typename Table>
	std::size_t query( implementation_type & impl, const std::string & str, Table * table = 0 );

	template<typename Handler>
	std::size_t query( implementation_type & impl, const std::string & str, Handler handler );

private:
	/// Private io_service used for performing operations in work_thread.
	boost::asio::io_service work_io_service;

	/// Work for the private io_service to perform. If we do not give the
	/// io_service some work to do then the io_service::run() function will exit
	/// immediately.
	boost::scoped_ptr<boost::asio::io_service::work> work;

	/// Thread used for running the work io_service's run loop.
	boost::scoped_ptr<boost::thread> work_thread;

};

} // namespace db
} // namespace boost

#endif // __DATABASE_SERVICE_HPP__

