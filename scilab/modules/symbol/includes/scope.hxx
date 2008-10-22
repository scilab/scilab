/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#include <iostream>
#include <map>
#include "symbol.hxx"
#include "alltypes.hxx"

using types::GenericType;

namespace symbol
{

	//template< class Entry_T, class Key_T = Symbol >
	class Scope
	{
	public:
		/** Constructor & Destructor */
		/** \brief Construct a Scope */
		explicit Scope()
		{
			_scope = new std::map<Symbol, GenericType*>();
			_name = "";
		}
		/** \brief Construct a named Scope i.e Namespace */
		explicit Scope(std::string name)
		{
			_scope = new std::map<Symbol, GenericType*>();
			_name = name;
		}

		~Scope()
		{
			delete _scope;
		}



		/** Associate value to key in the current scope. */
		void	put (Symbol key, GenericType &value)
		{
			GenericType *pOld = (*_scope)[key];
			if(pOld != NULL)
			{
				delete pOld;
			}
			(*_scope)[key] = &value;
		}

		/** If key was associated to some Entry_T in the open scopes, return the
		** most recent insertion. Otherwise return the empty pointer. */
		GenericType*	get (Symbol key) const
		{
			std::map<Symbol, GenericType*>::const_iterator it_scope;

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
			std::map<Symbol, GenericType*>::const_iterator it_scope;

			for(it_scope = _scope->begin() ; it_scope != _scope->end() ; ++it_scope)
			{
				if (_name != "")
				{
					ostr << _name << "::";
				}
				ostr << (*it_scope).first << " : ";
				(*it_scope).second->whoAmI();
				if((*it_scope).second->isDouble())
				{
					types::Double *pdbl = (*it_scope).second->getAsDouble();
					int iCols = pdbl->cols_get();
					int iRows = pdbl->rows_get();
					if(iRows == 1 && iCols == 1)
					{
						ostr << pdbl->real_get(0,0);
						if(pdbl->isComplex())
						{
							ostr << (pdbl->img_get(0,0) < 0 ? " " : " +") << pdbl->img_get(0,0) << "i";
						}

						ostr << std::endl;
					}
					else
					{
						ostr << "[ ";
						for(int r = 0 ; r < iRows ; r++)
						{
							for(int c = 0 ; c < iCols ; c++)
							{
								ostr << pdbl->real_get(r, c);
								if(pdbl->isComplex())
								{
									ostr << (pdbl->img_get(r, c) < 0 ? " " : " +") << pdbl->img_get(r, c) << "i";
								}

								if((c + 1) != iCols || (r + 1) != iRows)
								{
									ostr << " , ";
								}
							}
							ostr  << std::endl;
						}
						ostr << " ]" << std::endl;
					}				}
				else if((*it_scope).second->isInt())
				{
					types::Int *pi = (*it_scope).second->getAsInt();
					int iCols = pi->cols_get();
					int iRows = pi->rows_get();
					if(iRows == 1 && iCols == 1)
					{
						ostr << pi->real_get(0,0);
						if(pi->isComplex())
						{
							ostr << (pi->img_get(0,0) < 0 ? " " : " +") << pi->img_get(0,0) << "i";
						}

						ostr << std::endl;
					}
					else
					{
						ostr << "[ ";
						for(int r = 0 ; r < iRows ; r++)
						{
							for(int c = 0 ; c < iCols ; c++)
							{
								ostr << pi->real_get(r, c);
								if(pi->isComplex())
								{
									ostr << (pi->img_get(r, c) < 0 ? " " : " +") << pi->img_get(r, c) << "i";
								}

								if((c + 1) != iCols || (r + 1) != iRows)
								{
									ostr << " , ";
								}
							}
							ostr  << std::endl;
						}
						ostr << " ]" << std::endl;
					}
				}
				else if((*it_scope).second->isString())
				{
					types::String *psz = (*it_scope).second->getAsString();
					int iCols = psz->cols_get();
					int iRows = psz->rows_get();

					if(iRows == 1 && iCols == 1)
					{
						ostr << psz->string_get(0,0) << std::endl;
					}
					else
					{
						ostr << "[ ";
						for(int r = 0 ; r < iRows ; r++)
						{
							for(int c = 0 ; c < iCols ; c++)
							{
								ostr << psz->string_get(r, c);
								if((c + 1) != iCols || (r + 1) != iRows)
								{
									ostr << " , ";
								}

							}
							ostr  << std::endl;
						}
						ostr << " ]" << std::endl;
					}
				}
				else if((*it_scope).second->isBool())
				{
					types::Bool *pb = (*it_scope).second->getAsBool();
					int iCols = pb->cols_get();
					int iRows = pb->rows_get();
					if(iRows == 1 && iCols == 1)
					{
						ostr << (pb->bool_get(0,0) == true ? "true" : "false");
						ostr << std::endl;
					}
					else
					{
						ostr << "[ ";
						for(int r = 0 ; r < iRows ; r++)
						{
							for(int c = 0 ; c < iCols ; c++)
							{
								ostr << (pb->bool_get(r, c) == true ? "true" : "false");
								if((c + 1) != iCols || (r + 1) != iRows)
								{
									ostr << " , ";
								}
							}
							ostr  << std::endl;
						}
						ostr << " ]" << std::endl;
					}
				}
			}
		}

	private:
		std::map<Symbol, GenericType*>* 	_scope;
		std::string				_name;
	};

	inline std::ostream& operator<< (std::ostream& ostr,
		const Scope &scope)
	{
		scope.print(ostr);
		return ostr;
	}
}


#endif /* ! SYMBOL_SCOPE_HH */

