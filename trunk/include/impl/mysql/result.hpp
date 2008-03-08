#ifndef BOOST_DATABASE_IMPL_MYSQL_RESULT
#define BOOST_DATABASE_IMPL_MYSQL_RESULT

#include <mysql.h>
#include <boost/integer_traits.hpp>
#include "traits/field_type.hpp"
#include <vector>

namespace boost
{
namespace database
{
namespace impl
{
namespace mysql
{

class result_base
{
public:
	result_base( MYSQL_BIND & b ) : mysql_bind(b), is_null(0)
	{
		mysql_bind.is_null = &is_null;
	}

	virtual ~result_base() {}

	virtual void fetch() = 0;

protected:
	MYSQL_BIND & mysql_bind;
	my_bool is_null;
};

template<typename T>
class result : public result_base
{
public:
	result( MYSQL_BIND & b, T & v ) : result_base(b), value(v)
	{
		mysql_bind.buffer_type = field_type<T>::value;
		mysql_bind.buffer = (void*)( &copy );
		mysql_bind.buffer_length = sizeof(T);
		mysql_bind.is_unsigned = !integer_traits<T>::is_signed;
	}

	void fetch()
	{
		value = is_null ? 0 : copy;
	}

private:
	T copy;
	T & value;
};

template<>
class result<std::string> : public result_base
{
public:
	result( MYSQL_BIND & b, std::string & v ) : result_base(b), value(v)
	{
		buffer.resize( mysql_bind.buffer_length );
		mysql_bind.buffer_type = MYSQL_TYPE_STRING;
		mysql_bind.buffer = &buffer[0];
		mysql_bind.length = &length;
	}

	void fetch()
	{
		if( is_null )
			value.clear();
		else
			value = std::string( &buffer[0], length );
	}

private:
	std::vector<char> buffer;
	unsigned long int length;
	std::string & value;
};

} // end namespace mysql
} // end namespace impl
} // end namespace database
} // end namespace boost

#endif /*BOOST_DATABASE_IMPL_MYSQL_RESULT*/
