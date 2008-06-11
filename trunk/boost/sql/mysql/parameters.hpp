#ifndef BOOST_SQL_MYSQL_PARAMETERS_HPP
#define BOOST_SQL_MYSQL_PARAMETERS_HPP

#include <mysql.h>

#include <boost/sql/buffer.hpp>
#include <boost/sql/buffer_length.hpp>
#include <boost/sql/mysql/traits.hpp>
#include <boost/type_traits.hpp>

#include <boost/array.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/count_if.hpp>

#include <boost/fusion/include/mpl.hpp>
#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include <boost/type_traits/detail/bool_trait_def.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{
namespace detail
{

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_time,T,false)
BOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_time,gregorian::date,true)
BOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_time,posix_time::time_duration,true)
BOOST_TT_AUX_BOOL_TRAIT_SPEC1(is_time,posix_time::ptime,true)

} // end namespace detail
} // end namespace mysql
} // end namespace sql
} // end namespace boost

#include <boost/type_traits/detail/bool_trait_undef.hpp>

namespace boost
{
namespace sql
{
namespace mysql
{
namespace detail
{

using namespace mpl;


class bind_param_
{
public:
	bind_param_(MYSQL_BIND* param) :
		param_(param)
	{
	}

	template<typename T>
	void operator()(const T& value) const
	{
		param_->buffer_type = traits<T>::field_type;
		param_->buffer = const_cast<void*>(buffer(value) );
		param_->buffer_length = buffer_length(value);
		param_->is_unsigned = is_unsigned<T>::value;
		++param_;
	}

private:
	mutable MYSQL_BIND* param_;
};

template<std::size_t N>
struct mysql_bind
{
	MYSQL_BIND bind[N];

	void clear_param()
	{
		memset(bind, 0, sizeof(bind));
	}

	bind_param_ bind_param()
	{
		return bind_param_(bind);
	}

	void bind_params(MYSQL_STMT* stmt)
	{
		if (mysql_stmt_bind_param(stmt,bind))
			throw std::runtime_error(mysql_stmt_error(stmt));
	}
};

template<std::size_t N>
struct mysql_time
{
	array<MYSQL_TIME, N> times;
};

//contains_if
template<typename Seq, typename Pred>
struct contains_if
  : mpl::not_< boost::is_same<typename mpl::find_if<Seq,Pred>::type,
        typename mpl::end<Seq>::type> >
{
};

template<typename Seq>
class parameters : private
	if_<
		empty<Seq>,
		empty_base,
		typename inherit<
			mysql_bind<size<Seq>::value>,
			typename if_<
				contains_if<Seq, is_time<_> >,
				mysql_time<count_if<Seq, is_time<_> >::value>,
				empty_base
			>::type
		>::type
	>::type
{
	typedef typename fusion::result_of::as_vector<Seq>::type params_type;

public:
	void bind(MYSQL_STMT* stmt, const params_type& params)
	{
		this->clear_param();

		for_each(params, this->bind_param());

		this->bind_params(stmt);
	}
};

} // end namespace detail

using detail::parameters;

} // end namespace mysql
} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_MYSQL_PARAMETERS_HPP*/
