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

/**
 ** \file symbol/context.hh
 ** \brief Define class Context.
 */

#ifndef CONTEXT_HH
# define CONTEXT_HH

#include "stack.hh"
#include "heap.hh"

namespace symbol
{

  /** \brief Define class Context.
   */
  template< class Entry_T, class Key_T = Symbol >
  class Context
  {
  public :
    Context();/* {
      PrivateFunTable.scope_begin();
      PrivateVarTable.scope_begin();
      HeapFunTable.scope_begin();
      HeapVarTable.scope_begin();
      EnvFunTable.scope_begin();
      EnvVarTable.scope_begin();
    }
*/
  static Context* getInstance(void);

	/** Open a context scope i.e
     ** open the heap table one
     ** and the env table too. */
    void scope_begin();

    /** Close a context scope i.e
     ** close the heap table one
     ** and the env table too. */
		void scope_end();

    /** If key was associated to some Entry_T in the open scopes, return the
     ** most recent insertion. Otherwise return the empty pointer. */
    Entry_T*	get(Key_T key) const;

    /** If key was associated to some Entry_T in the open scopes, return the
     ** most recent insertion. Otherwise return the empty pointer. */
    Entry_T*	get_fun(Key_T key) const;

	private :
    static Stack<Entry_T, Key_T> PrivateFunTable;
    static Stack<Entry_T, Key_T> PrivateVarTable;
    static Heap<Entry_T, Key_T> HeapFunTable;
    static Heap<Entry_T, Key_T> HeapVarTable;
    static Stack<Entry_T, Key_T> EnvFunTable;
    static Stack<Entry_T, Key_T> EnvVarTable;

		static Context* me;
  };
}
#endif // !CONTEXT_HH
