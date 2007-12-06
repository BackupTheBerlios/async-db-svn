

Design:
~~~~~~~

database  database                   connection
driver    abstrac_database            impl (connection)

query     rowset                     statement
result    abstract_rowset             impl (statement)

record    row                        row

field     field + field_description  field + ?

error     exception                  error


Dependencies:
~~~~~~~~~~~~~

libmysqlclient15-dev
firebird2.0-dev
unixodbc-dev
libsqlite3-dev
libpq-dev

