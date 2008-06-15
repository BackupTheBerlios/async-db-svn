/**************************************************************
 * Copyright (c) 2008 Daniel Pfeifer                          *
 *                                                            *
 * Distributed under the Boost Software License, Version 1.0. *
 **************************************************************/
#ifndef BOOST_PP_IS_ITERATING
#ifndef BOOST_SQL_EXECUTABLE_HPP
#define BOOST_SQL_EXECUTABLE_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/fusion/mpl.hpp>

namespace boost
{
namespace sql
{

//inherit from this class to get operator()
//with elements from Seq as arguments

template<typename Impl, typename Seq, typename Enable = void>
struct executable
{
};

template<typename Impl, typename Seq>
struct executable<Impl, Seq,
		typename enable_if<typename mpl::empty<Seq>::type>::type>
{
	void operator()()
	{
		static_cast<Impl*> (this)->execute(Seq());
	}
};

#define BOOST_PP_FILENAME_1 <boost/sql/executable.hpp>
#define BOOST_PP_ITERATION_LIMITS (1, FUSION_MAX_VECTOR_SIZE)
#include BOOST_PP_ITERATE()

} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_EXECUTABLE_HPP*/

#else /*BOOST_PP_IS_ITERATING*/

#define N BOOST_PP_ITERATION()

template<typename Impl, typename Seq>
struct executable<Impl, Seq, typename enable_if<typename is_same<
typename mpl::size<Seq>::type, mpl::int_<N>::type>::type>::type>
{
	template<BOOST_PP_ENUM_PARAMS(N, typename T)>
	void operator()(BOOST_PP_ENUM_BINARY_PARAMS(N, T, arg))
	{
		static_cast<Impl*> (this)->execute(Seq(BOOST_PP_ENUM_PARAMS(N, arg)));
	}
};

#undef N

#endif /*BOOST_PP_IS_ITERATING*/
