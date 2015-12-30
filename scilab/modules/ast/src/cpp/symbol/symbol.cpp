/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef SYMBOL_HXX
#define SYMBOL_HXX

#include "symbol.hxx"

symbol::Symbol::string_set_type symbol::Symbol::_set;

namespace symbol
{
// Constructor
Symbol::Symbol (const std::string &s):
    _set_node (_set.insert(s).first)
{
}

// Accessor
const std::string& Symbol::getName () const
{
    return (*_set_node);
}

// Return the size of the Symbol map.
Symbol::size_type Symbol::getSize()
{
    return _set.size();
}

// Operators for better performances.
bool Symbol::operator== (const Symbol &rhs) const
{
    return &(*_set_node) == &(*rhs.getNode());
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

std::ostream& operator<< (std::ostream &ostr, const Symbol &the)
{
    return ostr << the.getName();
}

char **Symbol::getAll()
{
    string_set_type::const_iterator it;
    char **resultVector = new char*[getSize()];
    int i = 0;

    for (it = _set.begin() ; it != _set.end() ; ++it, ++i)
    {
        resultVector[i] = const_cast<char*>(it->c_str());
    }

    return resultVector;
}
}

#endif // !SYMBOL_SYMBOL_HXX
