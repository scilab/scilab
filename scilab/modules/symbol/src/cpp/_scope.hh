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

#ifndef SYMBOL_SCOPE_HH
# define SYMBOL_SCOPE_HH

# include <iostream>
# include <map>
# include "symbol.hh"

namespace symbol
{

	template< class Entry_T, class Key_T = Symbol >
	class Scope
	{
	public:
		/** Constructor & Destructor */
		/** \brief Construct a Scope */
		explicit Scope()
		{
			_scope = new std::map< Key_T, Entry_T* >();
			_name = "";
		}
		/** \brief Construct a named Scope i.e Namespace */
		explicit Scope(std::string name)
		{
			_scope = new std::map< Key_T, Entry_T* >();
			_name = name;
		}

		~Scope()
		{
			delete _scope;
		}



		/** Associate value to key in the current scope. */
		void	put (Key_T key, Entry_T &value)
		{
			(*_scope)[key] = &value;
		}

		/** If key was associated to some Entry_T in the open scopes, return the
		** most recent insertion. Otherwise return the empty pointer. */
		Entry_T*	get (Key_T key) const
		{
			typename std::map< Key_T, Entry_T* >::const_iterator
				it_scope;

			it_scope = (*_scope).find(key);
			if (it_scope == (*_scope).end())
				return 0;
			return (*it_scope).second;
		}

		/** Return name of current scope, use for namespace. */
		std::string get_name() const
		{
			return _name;
		}

		/** Send the content of this table on ostr in a readable manner, the top
		** of the stack being displayed last. */
		void	print (std::ostream& ostr) const
		{
			typename std::map< Key_T, Entry_T* >::const_iterator
				it_scope;

			for(it_scope = _scope->begin() ; it_scope != _scope->end() ; ++it_scope)
			{
				if (_name != "")
				{
					ostr << _name << "::";
				}
				ostr << (*it_scope).first << " : ";
				ostr << *((*it_scope).second) << std::endl;
			}
		}

	private:
		std::map< Key_T, Entry_T* >* 	_scope;
		std::string				_name;
	};

	template< class T, class Key_T >
	inline std::ostream& operator<< (std::ostream& ostr,
		const Scope<T, Key_T>& scope)
	{
		scope.print(ostr);
		return ostr;
	}
}


#endif /* ! SYMBOL_SCOPE_HH */

