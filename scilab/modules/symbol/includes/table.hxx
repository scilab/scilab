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

#include <iostream>
#include <list>
#include "symbol.hxx"
#include "scope.hxx"
#include "export_symbol.h"

namespace symbol
{

	/*-----------------------------------------------------------------------.
	| This implements tables for storing definitions of variables and        |
	| functions as a stack of dictionnaries.  Each time a scope is opened, a |
	| new dictionnary is added on the top of the stack; the dictionary is    |
	| removed when the scope is closed.  Lookup of symbols is donne in the   |
	| last added dictionnary first (LIFO).                                   |
	`-----------------------------------------------------------------------*/
	class EXTERN_SYMBOL Table
	{
	public:

		virtual ~Table()
		{
		}

		/** Associate value to key in the current scope. */
		virtual void put (const wstring& key, InternalType &value) = 0 ;

		/** If key was associated to some Entry_T in the open scopes, return the
		** most recent insertion. Otherwise return the empty pointer. */
		virtual InternalType*	get (const wstring& key) const = 0 ;

		/** Send the content of this table on ostr in a readable manner, the top
		** of the stack being displayed last. */
		void	print (std::wostream& ostr) const;

	protected:
		std::list<Scope*> l_scope;
	};

}


#endif /* ! SYMBOL_TABLE_HH */

