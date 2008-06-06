#ifndef BOOST_DATABASE_IMPL_MYSQL_TRAITS
#define BOOST_DATABASE_IMPL_MYSQL_TRAITS

#include <mysql.h>
#include <boost/cstdint.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{

template<enum_field_types FieldType>
struct traits_base
{
	static const enum_field_types field_type = FieldType;
};

template<typename T>
struct traits : traits_base<MYSQL_TYPE_STRING>
{
};

template<>
struct traits<int8_t> : traits_base<MYSQL_TYPE_TINY>
{
};

template<>
struct traits<uint8_t> : traits_base<MYSQL_TYPE_TINY>
{
};

template<>
struct traits<int16_t> : traits_base<MYSQL_TYPE_SHORT>
{
};

template<>
struct traits<uint16_t> : traits_base<MYSQL_TYPE_SHORT>
{
};

template<>
struct traits<int32_t> : traits_base<MYSQL_TYPE_LONG>
{
};

template<>
struct traits<uint32_t> : traits_base<MYSQL_TYPE_LONG>
{
};

template<>
struct traits<int64_t> : traits_base<MYSQL_TYPE_LONGLONG>
{
};

template<>
struct traits<uint64_t> : traits_base<MYSQL_TYPE_LONGLONG>
{
};

template<>
struct traits<float> : traits_base<MYSQL_TYPE_FLOAT>
{
};

template<>
struct traits<double> : traits_base<MYSQL_TYPE_DOUBLE>
{
};

} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /*BOOST_DATABASE_IMPL_MYSQL_TRAITS*/
