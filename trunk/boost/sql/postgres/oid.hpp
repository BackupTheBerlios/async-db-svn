#ifndef BOOST_SQL_POSTGRES_OID_HPP
#define BOOST_SQL_POSTGRES_OID_HPP

#include <postgres.h>
#include <postgres_ext.h>
#include <catalog/pg_type.h>
#include <boost/cstdint.hpp>
#include <boost/mpl/integral_c.hpp>

namespace boost
{
namespace sql
{
namespace postgres
{

template<typename T>
struct oid: mpl::integral_c<Oid, InvalidOid>
{
};

#define BOOST_SQL_DECLARE_OID(TYPE, OID) \
template<>struct oid<TYPE>: mpl::integral_c<Oid, OID> {};

BOOST_SQL_DECLARE_OID(bool, BOOLOID)
BOOST_SQL_DECLARE_OID(int16_t, INT2OID)
BOOST_SQL_DECLARE_OID(int32_t, INT4OID)
BOOST_SQL_DECLARE_OID(int64_t, INT8OID)
BOOST_SQL_DECLARE_OID(float, FLOAT4OID)
BOOST_SQL_DECLARE_OID(double, FLOAT8OID)
BOOST_SQL_DECLARE_OID(std::string, BYTEAOID)

#undef BOOST_SQL_DECLARE_OID

} // end namespace postgres
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_POSTGRES_OID_HPP*/
