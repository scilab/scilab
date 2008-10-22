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

#ifndef SYMBOL_TABLE_HH
# define SYMBOL_TABLE_HH

# include <iostream>
# include <list>
# include "symbol.hh"
# include "scope.hh"

namespace symbol
{

/*-----------------------------------------------------------------------.
| This implements tables for storing definitions of variables and        |
| functions as a stack of dictionnaries.  Each time a scope is opened, a |
| new dictionnary is added on the top of the stack; the dictionary is    |
| removed when the scope is closed.  Lookup of symbols is donne in the   |
| last added dictionnary first (LIFO).                                   |
`-----------------------------------------------------------------------*/
  template< class Entry_T, class Key_T = Symbol >
  class Table
  {
  public:

    virtual ~Table()
    {
    }

    /** Associate value to key in the current scope. */
    virtual void put (Key_T key, Entry_T &value) = 0 ;

    /** If key was associated to some Entry_T in the open scopes, return the
     ** most recent insertion. Otherwise return the empty pointer. */
    virtual Entry_T*	get (Key_T key) const = 0 ;

    /** Send the content of this table on ostr in a readable manner, the top
     ** of the stack being displayed last. */
    void	print (std::ostream& ostr) const
    {
      typename std::list< Scope<Entry_T, Key_T> >::const_iterator
	it_list_scope;
      typename std::list< Scope<Entry_T, Key_T> >::size_type	i;

      for (i = (*this).l_scope.size() - 1, it_list_scope = (*this).l_scope.begin();
	   it_list_scope != (*this).l_scope.end();
	   --i, ++it_list_scope)
	{
	  ostr << "Scope level: " << i << std::endl;
	  ostr << *it_list_scope;
	}
    }

  protected:
    std::list< Scope<Entry_T, Key_T> > l_scope;
  };

}


#endif /* ! SYMBOL_TABLE_HH */

