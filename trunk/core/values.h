//
// Copyright (C) 2004-2007 Maciej Sobczak, Stephen Hutton
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef SOCI_VALUES_H_INCLUDED
#define SOCI_VALUES_H_INCLUDED

#include "statement.h"
#include "into-type.h"
#include "use-type.h"

#include <sstream>

namespace db
{

class values
{
    friend class details::statement_impl;
    friend class details::into_type<values>;
    friend class details::use_type<values>;

public:

    values() : row_(NULL), currentPos_(0) {}

    eIndicator indicator(std::size_t pos) const;
    eIndicator indicator(std::string const &name) const;

    template <typename T>
    T get(std::size_t pos) const
    {
        if (row_)
        {
            return row_->get<T>(pos);
        }
        else if (*indicators_[pos] != eNull)
        {
            return get_from_uses<T>(pos);
        }
        else
        {
            std::ostringstream msg;
            msg << "Column at position "
                << static_cast<unsigned long>(pos)
                << " contains NULL value and no default was provided";
            throw soci_error(msg.str());
        }
    }

    template <typename T>
    T get(std::size_t pos, T const &nullValue) const
    {
        if (row_)
        {
            return row_->get<T>(pos, nullValue);
        }
        else if (*indicators_[pos] == eNull)
        {
            return nullValue;
        }
        else
        {
            return get_from_uses<T>(pos);
        }
    }

    template <typename T>
    T get(std::string const &name) const
    {
        return row_ ? row_->get<T>(name) : get_from_uses<T>(name);
    }

    template <typename T>
    T get(std::string const &name, T const &nullValue) const
    {
        return row_ ? row_->get<T>(name, nullValue)
            : get_from_uses<T>(name, nullValue);
    }

    template <typename T>
    values const & operator>>(T &value) const
    {
        value = row_->get<T>(currentPos_);
        ++currentPos_;
        return *this;
    }

    template <typename T>
    void set(std::string const &name, T &value, eIndicator indicator=eOK)
    {
        index_.insert(std::make_pair(name, uses_.size()));
        eIndicator* ind = new eIndicator(indicator);

        uses_.push_back(new details::use_type<T>(value, *ind, name));
        indicators_.push_back(ind);
    }

private:

    //TODO To make values generally usable outside of TypeConversionS,
    // these should be reference counted smart pointers
    row *row_;
    std::vector<details::standard_use_type*> uses_;
    std::map<details::use_type_base*, eIndicator*> unused_;
    std::vector<eIndicator*> indicators_;
    std::map<std::string, size_t> index_;

    // When TypeConversion::to() is called, a values object is created
    // without an underlying row object.  In that case, get_from_uses()
    // returns the underlying field values
    template <typename T>
    T get_from_uses(std::string const &name, T const &nullValue) const
    {
        std::map<std::string, size_t>::const_iterator pos = index_.find(name);
        if (pos != index_.end())
        {
            if (*indicators_[pos->second] == eNull)
            {
                return nullValue;
            }

            try
            {
                return get_from_uses<T>(pos->second);
            }
            catch (soci_error const &)
            {
                throw soci_error("Value named " + name + " was set using"
                    " a different type than the one passed to get()");
            }
        }
        throw soci_error("Value named " + name + " not found.");
    }

    template <typename T>
    T get_from_uses(std::string const &name) const
    {
        std::map<std::string, size_t>::const_iterator pos = index_.find(name);
        if (pos != index_.end())
        {
            if (*indicators_[pos->second] == eNull)
            {
                throw soci_error("Column " + name + " contains NULL value and"
                    " no default was provided");
            }

            try
            {
                return get_from_uses<T>(pos->second);
            }
            catch (soci_error const &)
            {
                throw soci_error("Value named " + name + " was set using"
                    " a different type than the one passed to get()");
            }
        }
        throw soci_error("Value named " + name + " not found.");
    }

    template <typename T>
    T get_from_uses(size_t pos) const
    {
        details::standard_use_type* u = uses_[pos];
        if (dynamic_cast<details::use_type<T>* >(u))
        {
            return *static_cast<T*>(u->get_data());
        }
        else
        {
            std::ostringstream msg;
            msg << "Value at position "
                << static_cast<unsigned long>(pos)
                << " was set using a different type"
                   " than the one passed to get()";
            throw soci_error(msg.str());
        }
    }

    row& get_row()
    {
        row_ = new row();
        return *row_;
    }

    // this is called by Statement::bind(values)
    void add_unused(details::use_type_base *u, eIndicator *i)
    {
        static_cast<details::standard_use_type*>(u)->convert_to_base();
        unused_.insert(std::make_pair(u, i));
    }

    // this is called by details::into_type<values>::clean_up()
    // and use_type<values>::clean_up()
    void clean_up()
    {
        delete row_;
        row_ = NULL;

        // delete any uses and indicators which were created  by set() but
        // were not bound by the Statement
        // (bound uses and indicators are deleted in Statement::clean_up())
        for (std::map<details::use_type_base*, eIndicator*>::iterator pos =
            unused_.begin(); pos != unused_.end(); ++pos)
        {
            delete pos->first;
            delete pos->second;
        }
    }

    mutable std::size_t currentPos_;
};

} // namespace soci

#endif
