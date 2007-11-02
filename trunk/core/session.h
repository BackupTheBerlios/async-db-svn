//
// Copyright (C) 2004-2007 Maciej Sobczak, Stephen Hutton
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SOCI_SESSION_H_INCLUDED
#define SOCI_SESSION_H_INCLUDED

#include <boost/noncopyable.hpp>
#include <ostream>
#include <sstream>
#include <string>

namespace db
{
class values;
struct backend_factory;

namespace details
{

class session_backend;
class statement_backend;
class rowid_backend;
class blob_backend;

} // namespace details


class session : private boost::noncopyable
{
public:
    session();
    session(backend_factory const &factory, std::string const & connectString);

    ~session();

    void open(backend_factory const &factory, std::string const & connectString);
    void close();
    void reconnect();

    // once and prepare are for syntax sugar only
    // details::once_type once;
    // details::prepare_type prepare;

    // even more sugar
    // template <typename T>
    // details::once_temp_type operator<<(T const &t) { return once << t; }

    std::ostringstream & get_query_stream();

    std::string get_last_query() const;

    details::statement_backend * make_statement_backend();
    details::rowid_backend * make_rowid_backend();
    details::blob_backend * make_blob_backend();

private:
    std::ostringstream query_stream_;

    std::string lastQuery_;

    backend_factory const *lastFactory_;
    std::string lastConnectString_;

    details::session_backend *backEnd_;
};

template<typename Service>
class basic_session : private boost::noncopyable
{
public:
	typedef typename Service::backend_type backend_type;

	basic_session() : /*once(this), prepare(this),*/ backEnd_(NULL) {}

	basic_session( std::string const & connectString ) /*: once(this), prepare(this)*/
	{
		backEnd_ = new backend_type( connectString );
	}

    ~basic_session()
    {
    	delete backEnd_;
    }

    void open( std::string const & connectString );
    void close();
    void reconnect();

    // once and prepare are for syntax sugar only
    // details::once_type once;
    // details::prepare_type prepare;

    // even more sugar
    // template <typename T>
    // details::once_temp_type operator<<(T const &t) { return once << t; }

    std::ostringstream & get_query_stream();

    std::string get_last_query() const;

    details::statement_backend * make_statement_backend();
    details::rowid_backend * make_rowid_backend();
    details::blob_backend * make_blob_backend();

private:
    std::ostringstream query_stream_;

    std::string lastQuery_;

    std::string lastConnectString_;

    backend_type * backEnd_;
};

} // namespace db

#endif
