//
// Copyright (C) 2004-2006 Maciej Sobczak, Stephen Hutton, David Courtney
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SOCI_SQLITE3_H_INCLUDED
#define SOCI_SQLITE3_H_INCLUDED

#include <vector>
#include "soci-backend.h"

namespace sqlite_api
{
#include <sqlite3.h>
}

namespace db
{

struct sqlite3_statement_backend;
struct sqlite3_standard_into_type_backend : details::standard_into_type_backend
{
    sqlite3_standard_into_type_backend(sqlite3_statement_backend &st)
        : statement_(st) {}

    virtual void define_by_pos(int &position,
                             void *data, details::eExchangeType type);

    virtual void pre_fetch();
    virtual void post_fetch(bool gotData, bool calledFromFetch,
                           eIndicator *ind);

    virtual void clean_up();

    sqlite3_statement_backend &statement_;

    void *data_;
    details::eExchangeType type_;
    int position_;
};

struct sqlite3_vector_into_type_backend : details::vector_into_type_backend
{
    sqlite3_vector_into_type_backend(sqlite3_statement_backend &st)
        : statement_(st) {}

    virtual void define_by_pos(int &position,
                             void *data, details::eExchangeType type);

    virtual void pre_fetch();
    virtual void post_fetch(bool gotData, eIndicator *ind);

    virtual void resize(std::size_t sz);
    virtual std::size_t size();

    virtual void clean_up();

    sqlite3_statement_backend &statement_;

    void *data_;
    details::eExchangeType type_;
    int position_;
};

struct sqlite3_standard_use_type_backend : details::standard_use_type_backend
{
    sqlite3_standard_use_type_backend(sqlite3_statement_backend &st)
        : statement_(st), buf_(0) {}

    virtual void bind_by_pos(int &position,
                           void *data, details::eExchangeType type);
    virtual void bind_by_name(std::string const &name,
                            void *data, details::eExchangeType type);

    virtual void pre_use(eIndicator const *ind);
    virtual void post_use(bool gotData, eIndicator *ind);

    virtual void clean_up();

    sqlite3_statement_backend &statement_;

    void *data_;
    details::eExchangeType type_;
    int position_;
    std::string name_;
    char *buf_;
};

struct sqlite3_vector_use_type_backend : details::vector_use_type_backend
{
    sqlite3_vector_use_type_backend(sqlite3_statement_backend &st)
        : statement_(st) {}

    virtual void bind_by_pos(int &position,
                           void *data, details::eExchangeType type);
    virtual void bind_by_name(std::string const &name,
                            void *data, details::eExchangeType type);

    virtual void pre_use(eIndicator const *ind);

    virtual std::size_t size();

    virtual void clean_up();

    sqlite3_statement_backend &statement_;

    void *data_;
    details::eExchangeType type_;
    int position_;
    std::string name_;
};

struct sqlite3_column
{
    std::string data_;
    bool isNull_;
    char * blobBuf_;
    std::size_t blobSize_;
};

typedef std::vector<sqlite3_column> sqlite3_row;
typedef std::vector<sqlite3_row> sqlite3_recordset;

struct sqlite3_session_backend;
struct sqlite3_statement_backend : details::statement_backend
{
    sqlite3_statement_backend(sqlite3_session_backend &session);

    virtual void alloc();
    virtual void clean_up();
    virtual void prepare(std::string const &query,
        details::eStatementType eType);
    void resetIfNeeded();

    virtual execFetchResult execute(int number);
    virtual execFetchResult fetch(int number);

    virtual int get_number_of_rows();

    virtual std::string rewrite_for_procedure_call(std::string const &query);

    virtual int prepare_for_describe();
    virtual void describe_column(int colNum, eDataType &dtype,
                                std::string &columnName);

    virtual sqlite3_standard_into_type_backend * make_into_type_backend();
    virtual sqlite3_standard_use_type_backend * make_use_type_backend();
    virtual sqlite3_vector_into_type_backend * make_vector_into_type_backend();
    virtual sqlite3_vector_use_type_backend * make_vector_use_type_backend();

    sqlite3_session_backend &session_;
    sqlite_api::sqlite3_stmt *stmt_;
    sqlite3_recordset dataCache_;
    sqlite3_recordset useData_;
    bool databaseReady_;
    bool boundByName_;
    bool boundByPos_;

private:
    execFetchResult loadRS(int totalRows);
    execFetchResult loadOne();
    execFetchResult bindAndExecute(int number);
};

struct sqlite3_rowid_backend : details::rowid_backend
{
    sqlite3_rowid_backend(sqlite3_session_backend &session);

    ~sqlite3_rowid_backend();

    unsigned long value_;
};

struct sqlite3_blob_backend : details::blob_backend
{
    sqlite3_blob_backend(sqlite3_session_backend &session);

    ~sqlite3_blob_backend();

    virtual std::size_t get_len();
    virtual std::size_t read(std::size_t offset, char *buf,
                             std::size_t toRead);
    virtual std::size_t write(std::size_t offset, char const *buf,
                              std::size_t toWrite);
    virtual std::size_t append(char const *buf, std::size_t toWrite);
    virtual void trim(std::size_t newLen);

    sqlite3_session_backend &session_;

    std::size_t set_data(char const *buf, std::size_t toWrite);

private:
    char *buf_;
    size_t len_;
};

struct sqlite3_session_backend : details::session_backend
{
    sqlite3_session_backend( const std::string & connectString );

    ~sqlite3_session_backend();

    void clean_up();

    virtual sqlite3_statement_backend * make_statement_backend();
    virtual sqlite3_rowid_backend * make_rowid_backend();
    virtual sqlite3_blob_backend * make_blob_backend();

    sqlite_api::sqlite3 * conn_;
};

struct sqlite3_backend_factory : backend_factory
{
    virtual sqlite3_session_backend * make_session(
        std::string const &connectString) const;
};

extern sqlite3_backend_factory const sqlite3_singleton;

} // namespace SOCI

#endif // SOCI_SQLITE3_H_INCLUDED
