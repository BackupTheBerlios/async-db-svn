#ifndef BOOST_DATABASE_IMPL_MYSQL_TRAITS
#define BOOST_DATABASE_IMPL_MYSQL_TRAITS

#include <mysql.h>
#include <boost/cstdint.hpp>

namespace boost
{
namespace database
{
namespace impl
{
namespace mysql
{

template<typename T>
struct field_type
{
};

template<>
struct field_type<int8_t>
{
	static const enum_field_types value = MYSQL_TYPE_TINY;
};

template<>
struct field_type<uint8_t>
{
	static const enum_field_types value = MYSQL_TYPE_TINY;
};

template<>
struct field_type<int16_t>
{
	static const enum_field_types value = MYSQL_TYPE_SHORT;
};

template<>
struct field_type<uint16_t>
{
	static const enum_field_types value = MYSQL_TYPE_SHORT;
};

template<>
struct field_type<int32_t>
{
	static const enum_field_types value = MYSQL_TYPE_LONG;
};

template<>
struct field_type<uint32_t>
{
	static const enum_field_types value = MYSQL_TYPE_LONG;
};

template<>
struct field_type<int64_t>
{
	static const enum_field_types value = MYSQL_TYPE_LONGLONG;
};

template<>
struct field_type<uint64_t>
{
	static const enum_field_types value = MYSQL_TYPE_LONGLONG;
};

template<>
struct field_type<float>
{
	static const enum_field_types value = MYSQL_TYPE_FLOAT;
};

template<>
struct field_type<double>
{
	static const enum_field_types value = MYSQL_TYPE_DOUBLE;
};

} // end namespace mysql
} // end namespace impl
} // end namespace database
} // end namespace boost

#endif /*BOOST_DATABASE_IMPL_MYSQL_TRAITS*/
