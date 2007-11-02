//
// Copyright (C) 2004-2007 Maciej Sobczak, Stephen Hutton
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define SOCI_SOURCE
#include "session.h"
#include "soci-backend.h"

#ifdef _MSC_VER
#pragma warning(disable:4355)
#endif

using namespace db;
using namespace db::details;

namespace // anonymous
{

void ensureConnected(session_backend *backEnd)
{
    if (backEnd == NULL)
    {
        throw soci_error("Session is not connected.");
    }
}

} // namespace anonymous

session::session() : /*once(this), prepare(this),*/ lastFactory_(NULL), backEnd_(NULL) {}

session::session(backend_factory const &factory,
    std::string const & connectString)
    : /*once(this), prepare(this),*/
      lastFactory_(&factory), lastConnectString_(connectString)
{
    backEnd_ = factory.make_session(connectString);
}

session::~session()
{
    delete backEnd_;
}

void session::open(backend_factory const &factory,
    std::string const & connectString)
{
    if (backEnd_ != NULL)
    {
        throw soci_error("Cannot open already connected session.");
    }

    backEnd_ = factory.make_session(connectString);
    lastFactory_ = &factory;
    lastConnectString_ = connectString;
}

void session::close()
{
    delete backEnd_;
    backEnd_ = NULL;
}

void session::reconnect()
{
    if (lastFactory_ == NULL)
    {
        throw soci_error("Cannot reconnect without previous connection.");
    }

    if (backEnd_ != NULL)
    {
        close();
    }

    backEnd_ = lastFactory_->make_session(lastConnectString_);
}

std::ostringstream & session::get_query_stream()
{
    return query_stream_;
}

std::string session::get_last_query() const
{
    return lastQuery_;
}

statement_backend * session::make_statement_backend()
{
    ensureConnected(backEnd_);

    return backEnd_->make_statement_backend();
}

rowid_backend * session::make_rowid_backend()
{
    ensureConnected(backEnd_);

    return backEnd_->make_rowid_backend();
}

blob_backend * session::make_blob_backend()
{
    ensureConnected(backEnd_);

    return backEnd_->make_blob_backend();
}
