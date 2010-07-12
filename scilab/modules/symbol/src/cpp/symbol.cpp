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
# define SYMBOL_HXX

#include "symbol.hxx"

symbol::Symbol::string_set_type symbol::Symbol::_set;

namespace symbol
{
  // Constructor
  Symbol::Symbol (const std::wstring &s):
    _set_node (_set.insert(s).first)
  {
  }

  // Accessor
  const std::wstring& Symbol::name_get () const
  {
    return (*_set_node);
  }

  // Return the size of the Symbol map.
  Symbol::size_type Symbol::map_size ()
  {
    return _set.size();
  }

  // Operators for better performances.
  bool Symbol::operator== (const Symbol &rhs) const
  {
    return this->name_get() == rhs.name_get();
  }

  bool Symbol::operator!= (const Symbol &rhs) const
  {
    return this->name_get() != rhs.name_get();
  }

  bool Symbol::operator<  (const Symbol &rhs) const
  {
    return this->name_get() < rhs.name_get();
  }

  std::wostream& operator<< (std::wostream &ostr, const Symbol &the)
  {
    return ostr << the.name_get();
  }
}

#endif // !SYMBOL_SYMBOL_HXX
