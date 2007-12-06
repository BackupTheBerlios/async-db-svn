

Design:
~~~~~~~

database  database                   connection
driver    abstrac_database            impl (connection)
error     exception                  error
field     field + field_description  field + ?
query     rowset                     statement
record    row                        row
result    abstract_rowset             impl (statement)


Dependencies:
~~~~~~~~~~~~~

libmysqlclient15-dev
firebird2.0-dev
unixodbc-dev
libsqlite3-dev
libpq-dev

