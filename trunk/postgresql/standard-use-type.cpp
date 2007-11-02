//
// Copyright (C) 2004-2006 Maciej Sobczak, Stephen Hutton
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#define SOCI_POSTGRESQL_SOURCE
#include "soci-postgresql.h"
#include "rowid.h"
#include <libpq/libpq-fs.h> // libpq
#include <cctype>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <limits>
#include <sstream>

#ifdef SOCI_PGSQL_NOPARAMS
#define SOCI_PGSQL_NOBINDBYNAME
#endif // SOCI_PGSQL_NOPARAMS

#ifdef _MSC_VER
#pragma warning(disable:4355 4996)
#define snprintf _snprintf
#else
using std::snprintf;
#endif

using namespace db;
using namespace db::details;


void postgresql_standard_use_type_backend::bind_by_pos(
    int &position, void *data, eExchangeType type)
{
    data_ = data;
    type_ = type;
    position_ = position++;
}

void postgresql_standard_use_type_backend::bind_by_name(
    std::string const &name, void *data, eExchangeType type)
{
    data_ = data;
    type_ = type;
    name_ = name;
}

void postgresql_standard_use_type_backend::pre_use(eIndicator const *ind)
{
    if (ind != NULL && *ind == eNull)
    {
        // leave the working buffer as NULL
    }
    else
    {
        // allocate and fill the buffer with text-formatted client data
        switch (type_)
        {
        case eXChar:
            {
                buf_ = new char[2];
                buf_[0] = *static_cast<char*>(data_);
                buf_[1] = '\0';
            }
            break;
        case eXCString:
            {
                cstring_descriptor *strDescr
                    = static_cast<cstring_descriptor *>(data_);

                std::size_t len = std::strlen(strDescr->str_);
                buf_ = new char[len + 1];
                std::strcpy(buf_, strDescr->str_);
            }
            break;
        case eXStdString:
            {
                std::string *s = static_cast<std::string *>(data_);
                buf_ = new char[s->size() + 1];
                std::strcpy(buf_, s->c_str());
            }
            break;
        case eXShort:
            {
                std::size_t const bufSize
                    = std::numeric_limits<short>::digits10 + 3;
                buf_ = new char[bufSize];
                snprintf(buf_, bufSize, "%d",
                    static_cast<int>(*static_cast<short*>(data_)));
            }
            break;
        case eXInteger:
            {
                std::size_t const bufSize
                    = std::numeric_limits<int>::digits10 + 3;
                buf_ = new char[bufSize];
                snprintf(buf_, bufSize, "%d",
                    *static_cast<int*>(data_));
            }
            break;
        case eXUnsignedLong:
            {
                std::size_t const bufSize
                    = std::numeric_limits<unsigned long>::digits10 + 2;
                buf_ = new char[bufSize];
                snprintf(buf_, bufSize, "%lu",
                    *static_cast<unsigned long*>(data_));
            }
            break;
        case eXDouble:
            {
                // no need to overengineer it (KISS)...

                std::size_t const bufSize = 100;
                buf_ = new char[bufSize];

                snprintf(buf_, bufSize, "%.20g",
                    *static_cast<double*>(data_));
            }
            break;
        case eXStdTm:
            {
                std::size_t const bufSize = 20;
                buf_ = new char[bufSize];

                std::tm *t = static_cast<std::tm *>(data_);
                snprintf(buf_, bufSize, "%d-%02d-%02d %02d:%02d:%02d",
                    t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
                    t->tm_hour, t->tm_min, t->tm_sec);
            }
            break;
        case eXRowID:
            {
                // RowID is internally identical to unsigned long

                rowid *rid = static_cast<rowid *>(data_);
                postgresql_rowid_backend *rbe
                    = static_cast<postgresql_rowid_backend *>(
                        rid->get_backend());

                std::size_t const bufSize
                    = std::numeric_limits<unsigned long>::digits10 + 2;
                buf_ = new char[bufSize];

                snprintf(buf_, bufSize, "%lu", rbe->value_);
            }
            break;

        default:
            throw soci_error("Use element used with non-supported type.");
        }
    }

    if (position_ > 0)
    {
        // binding by position
        statement_.useByPosBuffers_[position_] = &buf_;
    }
    else
    {
        // binding by name
        statement_.useByNameBuffers_[name_] = &buf_;
    }
}

void postgresql_standard_use_type_backend::post_use(
    bool /* gotData */, eIndicator * /* ind */)
{
    // TODO: if postgresql_ allows to *get* data via this channel,
    // write it back to client buffers (variable)

    // clean up the working buffer, it might be allocated anew in
    // the next run of preUse
    clean_up();
}

void postgresql_standard_use_type_backend::clean_up()
{
    if (buf_ != NULL)
    {
        delete [] buf_;
        buf_ = NULL;
    }
}
