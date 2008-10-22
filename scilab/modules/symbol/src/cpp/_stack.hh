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

#ifndef SYMBOL_STACK_HH
# define SYMBOL_STACK_HH

# include <iostream>
# include <list>
# include <map>
# include <cassert>
# include "table.hh"

namespace symbol
{

/*-----------------------------------------------------------------------.
| This implements stacks for storing definitions of variables and        |
| functions as a stack of dictionnaries.  Each time a scope is opened, a |
| new dictionnary is added on the top of the stack; the dictionary is    |
| removed when the scope is closed.  Lookup of symbols is donne in the   |
| last added dictionnary first (LIFO).                                   |
`-----------------------------------------------------------------------*/
  template< class Entry_T, class Key_T = Symbol >
  class Stack : public Table< Entry_T, Key_T >
  {
  public:
    /** Open a new scope */
    void	scope_begin()
    {
      this->l_scope.push_front(*new Scope<Entry_T, Key_T>());
    }

    /** Close the last scope, forgetting everything since the latest
     **	scope_begin (). */
    void	scope_end()
    {
      this->l_scope.pop_front();
    }

    /** Associate value to key in the current scope. */
    void	put (Key_T key, Entry_T &value)
    {
      (this->l_scope.front()).put(key, value);
    }

    /** If key was associated to some Entry_T in the open scopes, return the
     ** most recent insertion. Otherwise return the empty pointer. */
    Entry_T*	get (Key_T key) const
    {
      Entry_T* result = 0;

      typename std::list< Scope<Entry_T, Key_T> >::const_iterator
	it_list_scope;

      for (it_list_scope = this->l_scope.begin();
	   it_list_scope != this->l_scope.end();
	   ++it_list_scope)
	{
	  result = (*it_list_scope).get(key);
	  if (result == 0)
	    continue ;
	  return result;
	}
      return result;
    }

  };

  template< class T, class Key_T >
  inline std::ostream& operator<< (std::ostream& ostr,
				   const Stack<T, Key_T>& tbl)
  {
    tbl.print (ostr);
    return ostr;
  }
}


#endif /* ! SYMBOL_STACK_HH */

