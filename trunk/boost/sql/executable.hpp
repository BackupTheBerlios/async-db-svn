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
//TODO: use preprocessor to generate

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

template<typename Impl, typename Seq>
struct executable<Impl, Seq, typename enable_if<typename is_same<
		typename mpl::size<Seq>::type, mpl::int_<1>::type>::type>::type>
{
	template<typename T0>
	void operator()(T0 arg0)
	{
		static_cast<Impl*> (this)->execute(Seq(arg0));
	}
};

template<typename Impl, typename Seq>
struct executable<Impl, Seq, typename enable_if<typename is_same<
		typename mpl::size<Seq>::type, mpl::int_<2>::type>::type>::type>
{
	template<typename T0, typename T1>
	void operator()(T0 arg0, T1 arg1)
	{
		static_cast<Impl*> (this)->execute(Seq(arg0, arg1));
	}
};

template<typename Impl, typename Seq>
struct executable<Impl, Seq, typename enable_if<typename is_same<
		typename mpl::size<Seq>::type, mpl::int_<3>::type>::type>::type>
{
	template<typename T0, typename T1, typename T2>
	void operator()(T0 arg0, T1 arg1, T2 arg2)
	{
		static_cast<Impl*> (this)->execute(Seq(arg0, arg1, arg2));
	}
};

} // end namespace sql
} // end namespace boost

#endif /*BOOST_SQL_EXECUTABLE_HPP*/
