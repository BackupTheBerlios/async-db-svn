//
// Copyright (C) 2004-2007 Maciej Sobczak, Stephen Hutton
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_OPTIONAL_H_INCLUDED
#define BOOST_OPTIONAL_H_INCLUDED

#include "type-conversion-traits.h"
#include "exchange-traits.h"

#include <boost/optional.hpp>

namespace db
{

// simple fall-back for boost::optional
template <typename T>
struct type_conversion<boost::optional<T> >
{
    typedef typename type_conversion<T>::base_type base_type;

    static void from_base(base_type const &in, eIndicator ind,
        boost::optional<T> &out)
    {
        if (ind == eNull)
        {
            out.reset();
        }
        else
        {
            T tmp;
            type_conversion<T>::from_base(in, ind, tmp);
            out = tmp;
        }
    }

    static void to_base(boost::optional<T> &in,
        base_type &out, eIndicator &ind)
    {
        if (in.is_initialized())
        {
            type_conversion<T>::to_base(in.get(), out, ind);
        }
        else
        {
            ind = eNull;
        }
    }
};

} // namespace soci

#endif // BOOST_OPTIONAL_H_INCLUDED
