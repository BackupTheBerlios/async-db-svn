#ifndef BOOST_DATABASE_DETAIL_ERROR_HPP_INCLUDED
#define BOOST_DATABASE_DETAIL_ERROR_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <string>
#include <sstream>
#include <stdexcept>

namespace boost
{
namespace database
{

class basis_error : public std::runtime_error
{
public:
	basis_error(int num, const std::string & msg) :
		std::runtime_error(msg), m_code(num)
	{
	}

	virtual ~basis_error() throw()
	{
	}

	const int code() const throw()
	{
		return m_code;
	}

	const char * message() const throw()
	{
		return std::runtime_error::what();
	}

	const char * what() const throw()
	{
		if (m_what.empty() )
		{
			try
			{
				std::stringstream str;
				str << "Error " << m_code << ": " << std::runtime_error::what();
				m_what = str.str();
			}
			catch (...)
			{
				return std::runtime_error::what();
			}
		}
		return m_what.c_str();
	}

private:
	int m_code;
	mutable std::string m_what;
};

} // end namespace database
} // end namespace boost

#endif /*BOOST_DATABASE_DETAIL_ERROR_HPP_INCLUDED*/
