/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef SYMBOL_HXX
#define SYMBOL_HXX

#include "symbol.hxx"

symbol::Symbol::string_set_type symbol::Symbol::_set;
std::mutex symbol::Symbol::_set_mutex;

namespace symbol
{
const std::wstring* Symbol::intern(const std::wstring& value)
{
    std::lock_guard<std::mutex> lock(_set_mutex);
    return &(*_set.insert(value).first);
}

// Constructor
Symbol::Symbol (const std::wstring &s):
    _set_node (intern(s))
{
}

// Accessor
const std::wstring& Symbol::getName () const
{
    return *_set_node;
}

// Return the size of the Symbol map.
Symbol::size_type Symbol::getSize()
{
    std::lock_guard<std::mutex> lock(_set_mutex);
    return _set.size();
}

// Operators for better performances.
bool Symbol::operator== (const Symbol &rhs) const
{
    return _set_node == rhs.getNode();
}

bool Symbol::operator!= (const Symbol &rhs) const
{
    return !(*this == rhs);
}

bool Symbol::operator<(const Symbol &rhs) const
{
    // compare values instead of pointers to preserve consistency across runs, platforms, etc..
    return *_set_node < *rhs.getNode();
}

std::wostream& operator<< (std::wostream &ostr, const Symbol &the)
{
    return ostr << the.getName();
}

wchar_t **Symbol::getAll()
{
    std::lock_guard<std::mutex> lock(_set_mutex);
    string_set_type::const_iterator it;
    wchar_t **resultVector = new wchar_t*[_set.size()];
    int i = 0;

    for (it = _set.begin() ; it != _set.end() ; ++it, ++i)
    {
        resultVector[i] = const_cast<wchar_t*>(it->c_str());
    }

    return resultVector;
}
}

#endif // !SYMBOL_SYMBOL_HXX
