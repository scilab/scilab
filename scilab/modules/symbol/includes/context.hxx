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

#include "stack.hxx"
#include "heap.hxx"

namespace symbol
{

  /** \brief Define class Context.
   */
  class Context
  {
	public :
		Context();
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
		GenericType*	get(Symbol key) const;

		/** If key was associated to some Entry_T in the open scopes, return the
		** most recent insertion. Otherwise return the empty pointer. */
		GenericType*	get_fun(Symbol key) const;

		/*print all tables*/
		void print();

		/*add symbol and value in the stack*/
		bool put(Symbol key, GenericType &type);

	private :
		Stack PrivateFunTable;
		Stack PrivateVarTable;
		Heap HeapFunTable;
		Heap HeapVarTable;
		Stack EnvFunTable;
		Stack EnvVarTable;

		static Context* me;
	};
}
#endif // !CONTEXT_HH
