#ifndef BOOST_SQL_STATEMENT_HPP
#define BOOST_SQL_STATEMENT_HPP

#include <boost/config.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/size.hpp>

#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/mpl.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>

#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/view/iterator_range.hpp>
#include <boost/fusion/iterator/advance.hpp>

#include <iostream>
#include <boost/fusion/sequence/io.hpp>

//#ifdef BOOST_HAS_VARIADIC_TMPL
//
//#define BOOST_SQL_TEMPLATE_PARAMS typename... Args
//#define BOOST_SQL_OPERATOR_PARAMS Args... args
//#define BOOST_SQL_BASE_TEMPL_PARAMS Args...
//#define BOOST_SQL_BASE_OPRTR_PARAMS args...
//
//#else

#include <boost/preprocessor/repetition/enum.hpp>

#define BOOST_SQL_TEMPLATE_PARAM(z,n,data) typename T##n=mpl::na
#define BOOST_SQL_TEMPLATE_PARAMS BOOST_PP_ENUM(7,BOOST_SQL_TEMPLATE_PARAM,)
// Expands to typename T0 = mpl::na, typename T1 = mpl::na

#define BOOST_SQL_OPERATOR_PARAM(z,n,data) T##n arg##n=mpl::na()
#define BOOST_SQL_OPERATOR_PARAMS BOOST_PP_ENUM(7,BOOST_SQL_OPERATOR_PARAM,)
// Expands to e.g. T0 arg0 = mpl::na(), T1 arg1 = mpl::na()

#define BOOST_SQL_BASE_TEMPL_PARAM(z,n,data) T##n
#define BOOST_SQL_BASE_TEMPL_PARAMS BOOST_PP_ENUM(7,BOOST_SQL_BASE_TEMPL_PARAM,)
// Expands to e.g. T0,T1

#define BOOST_SQL_BASE_OP_APPLY_PARAM(z,n,data) arg##n
#define BOOST_SQL_BASE_OP_APPLY_PARAMS BOOST_PP_ENUM(7,BOOST_SQL_BASE_OP_APPLY_PARAM,)
// Expands to e.g. arg0,arg1

//#endif

namespace boost
{
namespace sql
{

template<typename Impl, BOOST_SQL_TEMPLATE_PARAMS>
class basic_statement
{
	typedef typename mpl::vector<BOOST_SQL_BASE_TEMPL_PARAMS>::type mpl_vector;
	typedef typename fusion::vector<BOOST_SQL_BASE_TEMPL_PARAMS> fusion_vector;

	typedef typename mpl::size<mpl_vector>::type size;
	typedef typename fusion::result_of::begin<fusion_vector>::type begin;
	typedef typename fusion::result_of::advance<begin, size>::type end;

public:
	typedef typename fusion::iterator_range<begin, end> param_t;

public:
	void operator()(BOOST_SQL_OPERATOR_PARAMS)
	{
		fusion_vector vec = fusion::make_vector(BOOST_SQL_BASE_OP_APPLY_PARAMS);

		begin b(vec);
		end e(vec);

		param_t params(b, e);

		std::cout << params << std::endl;

		static_cast<Impl*>(this)->execute(params);
	}
};

} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_STATEMENT_HPP*/
