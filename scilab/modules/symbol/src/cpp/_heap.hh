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

#ifndef SYMBOL_HEAP_HH
# define SYMBOL_HEAP_HH

# include <iostream>
# include <list>
# include "table.hh"

#define GLOBAL "global"

namespace symbol
{

/*-----------------------------------------------------------------------.
| This implements heaps for storing definitions of variables and         |
| functions as a list of dictionnaries.  Each time a scope is opened, a  |
| new dictionnary is added on the top of the heap; the dictionary is     |
| removed when the scope is closed.  Lookup of symbols is donne in the   |
| last added dictionnary first (LIFO).                                   |
`-----------------------------------------------------------------------*/
  template< class Entry_T, class Key_T = Symbol >
  class Heap : public Table< Entry_T, Key_T >
  {
  public:
    /**

     */
    Heap()
    {
      scope_begin(GLOBAL);
    }

    /** Open a new scope */
    void	scope_begin(std::string name)
    {
      this->l_scope.push_front(*new Scope<Entry_T, Key_T>(name));
    }

    /** Close the last scope, forgetting everything since the latest
     **	scope_begin (). */
    void	scope_end(std::string name)
    {
      this->l_scope.pop_front();
    }

    /*
    ** Global namespace direct access.
    ** {
    */

    /** Associate value to key in the global scope. */
    void	put (Key_T key, Entry_T &value)
    {
      (this->l_scope.back()).put(key, value);
    }

    /** \brief If key was associated to some Entry_T in
     ** the "global"/last namespace, return the
     ** most recent insertion. Otherwise return the empty pointer.
     ** \param key : the key to look for
     */
    Entry_T*	get (Key_T key) const
    {
      Entry_T* result = 0;

      result = (this->l_scope.back()).get(key);
      return result;
    }

    /*
    ** }
    ** END Global Namespace
    */

    /** \brief Associate a value to a key in the given namespace.
     ** If the Namespace does not exists it will be automatically
     ** created.
     ** \param name : the name of the namespace.
     ** \param key : the key.
     ** \param value : the value associated to the key.
    */
    void	put(std::string name, Key_T key, Entry_T &value)
    {
      typename std::list< Scope<Entry_T, Key_T> >::iterator
	it_list_scope;

      for (it_list_scope = this->l_scope.begin();
	   it_list_scope != this->l_scope.end();
	   ++it_list_scope)
	{
	  if ((*it_list_scope).get_name() == name)
	    {
	      (*it_list_scope).put(key, value);
	      return ;
	    }
	}
      scope_begin(name);
      (this->l_scope.front()).put(key, value);
    }

    /** \brief If key was associated to some Entry_T in
     ** the "global"/last namespace, return the
     ** most recent insertion. Otherwise return the empty pointer.
     ** \param key : the key to look for
     */
    Entry_T*	get (std::string name, Key_T key) const
    {
      typename std::list< Scope<Entry_T, Key_T> >::const_iterator
	it_list_scope;

      for (it_list_scope = this->l_scope.begin();
	   it_list_scope != this->l_scope.end();
	   ++it_list_scope)
	{
	  if ((*it_list_scope).get_name() == name)
	    {
	      return (*it_list_scope).get(key);
	    }
	}
      return NULL;
    }

  };

  template< class T, class Key_T >
  inline std::ostream& operator<< (std::ostream& ostr,
				   const Heap<T, Key_T>& tbl)
  {
    tbl.print (ostr);
    return ostr;
  }
}


#endif /* ! SYMBOL_HEAP_HH */

