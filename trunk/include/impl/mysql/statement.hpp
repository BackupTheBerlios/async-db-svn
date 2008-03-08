#ifndef __MYSQL_RESULT_HPP__
#define __MYSQL_RESULT_HPP__

#include "traits/field_type.hpp"
#include <mysql.h>
#include <boost/optional.hpp>
#include <boost/cstdint.hpp>
#include <boost/unordered_map.hpp>
#include <boost/call_traits.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <limits>
#include <iostream>
#include "result.hpp"
#include <map>

namespace boost
{
namespace database
{
namespace impl
{
namespace mysql
{

class statement
{
public:
	explicit statement(MYSQL * db) :
		mysql(db), stmt(0), meta(0)
	{
	}

	~statement()
	{
		// must iterate trough leftover result sets from multi-selects or stored procedures
		// if this isn't done subsequent queries will fail with "Commands out of sync"
		while( mysql_next_result(mysql) == 0 )
		{
			MYSQL_RES * res = mysql_store_result( mysql );
			if( res )
				mysql_free_result( res );
		}

		if( stmt )
			mysql_stmt_close( stmt );

		if( meta )
			mysql_free_result( meta );
	}

	template<typename Statement>
	void prepare( Statement query )
	{
		cleanup();

		BOOST_ASSERT( !stmt ); //should be cleaned up
		BOOST_ASSERT( !meta ); //should be cleaned up

		stmt = mysql_stmt_init( mysql );
		if( !stmt )
			throw my_error(mysql);

		if( mysql_stmt_prepare(stmt,query(),strlen(query())) )
			throw my_error(stmt);

		unsigned long param_count = mysql_stmt_param_count( stmt );
		m_params.resize( param_count );

		meta = mysql_stmt_result_metadata( stmt );
		if( !meta )
			return;

		unsigned int num_fields = mysql_num_fields( meta );
		m_results.resize( num_fields );
		memset( &m_results[0], 0, num_fields * sizeof(MYSQL_BIND) );

		MYSQL_FIELD * fieldInfo;
		int i = 0;
		while( (fieldInfo = mysql_fetch_field(meta)) )
			m_results[i++].buffer_length = fieldInfo->length + 1;
	}

	void cleanup()
	{
		// must iterate trough leftover result sets from multi-selects or stored procedures
		// if this isn't done subsequent queries will fail with "Commands out of sync"
		while( mysql_next_result(mysql) == 0 )
		{
			MYSQL_RES *res = mysql_store_result( mysql );
			if( res )
				mysql_free_result( res );
		}

		if( stmt )
		{
			mysql_stmt_close( stmt );
			stmt = 0;
		}

		if( meta )
		{
			mysql_free_result( meta );
			meta = 0;
		}

		m_params.clear();
		m_results.clear();
	}

	template<typename... Args>
	bool fetch( Args&... args )
	{
		bind_result( 0, args... );
		
		if (mysql_stmt_bind_result(stmt, &m_results[0]) )
			throw my_error(stmt);

		switch (mysql_stmt_fetch(stmt) )
		{
		case 1:
			throw my_error(stmt);
		case MYSQL_NO_DATA:
			return false;
		}

		BOOST_FOREACH( result_map_type::value_type r, results )
		{
			r.second->fetch();
		}
		return true;
	}

	template<typename... Args>
	void exec( Args&... args )
	{
		BOOST_ASSERT( stmt );

		if( mysql_stmt_reset(stmt) )
			throw my_error(stmt);

		if( !m_params.empty() )
		{
			bind_param( &m_params[0], args... );
			
			if( mysql_stmt_bind_param(stmt,&m_params[0]) )
				throw my_error(stmt);
		}

		if( mysql_stmt_execute(stmt) )
			throw my_error(stmt);

		if( !m_results.empty() )
		{
			if( mysql_stmt_store_result(stmt) )
				throw my_error(stmt);
		}

		//rowsAffected = mysql_stmt_affected_rows( stmt );
		//size = !meta ? -1 : mysql_stmt_num_rows( stmt );
		//lastInsertId = mysql_stmt_insert_id( stmt );
	}
	
private:
	template<size_t N>
	void bind_param( MYSQL_BIND * bnd, const char (&value)[N] )
	{
		bnd->buffer_type = MYSQL_TYPE_STRING;
		bnd->buffer = (void*)value;
		bnd->buffer_length = N;
	}

	void bind_param( MYSQL_BIND * bnd, const char * value )
	{
		bnd->buffer_type = MYSQL_TYPE_STRING;
		bnd->buffer = (void*)value;
		bnd->buffer_length = strlen(value);
	}

	void bind_param( MYSQL_BIND * bnd, const std::string & value )
	{
		bnd->buffer_type = MYSQL_TYPE_STRING;
		bnd->buffer = (void*)value.c_str();
		bnd->buffer_length = value.length();
	}

	template<typename T>
	void bind_param( MYSQL_BIND * bnd, const T & value )
	{
		bnd->buffer_type = field_type<T>::value;
		bnd->buffer = (void*)( &value );
		bnd->buffer_length = sizeof(T);
		bnd->is_unsigned = !std::numeric_limits<T>::is_signed;
	}
	
	template<typename T, typename... Args>
	void bind_param( MYSQL_BIND * bind, T first, Args&... next )
	{
		bind_param( bind, first );
		bind_param( bind+1, next... );
	}
	
	void bind_param(MYSQL_BIND*){}

private:
	template<typename T>
	void bind_result(int position, T & value)
	{
		results[position].reset( new result<T>(m_results[position],value) );
	}

	template<typename T, typename... Args>
	void bind_result( int pos, T & first, Args&... following )
	{
		bind_result( pos, first );
		bind_result( pos+1, following... );
	}

private:
	typedef std::map<int,shared_ptr<result_base> > result_map_type;
	result_map_type results;

	MYSQL * mysql;
	MYSQL_RES * meta;
	MYSQL_STMT * stmt;

	std::vector<MYSQL_BIND> m_results;
	std::vector<MYSQL_BIND> m_params;
};

} // end namespace mysql
} // end namespace impl
} // end namespace database
} // end namespace boost

#endif /*__MYSQL_RESULT_HPP__*/
