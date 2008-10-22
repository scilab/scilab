/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "context.hxx"

namespace symbol
{
	Context* Context::me;

	Context::Context() 
	{
		PrivateFunTable.scope_begin();
		PrivateVarTable.scope_begin();
		HeapFunTable.scope_begin();
		HeapVarTable.scope_begin();
		EnvFunTable.scope_begin();
		EnvVarTable.scope_begin();
	}

	Context* Context::getInstance(void)
  {
		if (me == 0)
		{
			me = new Context();
		}
		return me;
  }

	void Context::scope_begin() {
		PrivateFunTable.scope_begin();
		PrivateVarTable.scope_begin();
		HeapFunTable.scope_begin();
		HeapVarTable.scope_begin();
		EnvFunTable.scope_begin();
		EnvVarTable.scope_begin();
	}

	void Context::scope_end() {
		PrivateFunTable.scope_end();
		PrivateVarTable.scope_end();
		EnvFunTable.scope_end();
		EnvVarTable.scope_end();
		HeapFunTable.scope_end();
		HeapVarTable.scope_end();
	}

	GenericType*	Context::get(Symbol key) const
	{
		// FIXME
		return EnvVarTable.get(key);
	}

	GenericType*	Context::get_fun(Symbol key) const
	{
		return EnvFunTable.get(key);
		// FIXME
	}

	bool Context::put(Symbol key, GenericType &type)
	{
		// FIXME
		EnvVarTable.put(key, type);
		return true;
	}

	void Context::print()
	{
		std::cout << "PrivateFunTable : " << std::endl;
		std::cout << PrivateFunTable << std::endl << std::endl;
		std::cout << "PrivateVarTable : " << std::endl;
		std::cout << PrivateVarTable << std::endl << std::endl;
		std::cout << "EnvFunTable : " << std::endl;
		std::cout << EnvFunTable << std::endl << std::endl;
		std::cout << "EnvVarTable : " << std::endl;
		std::cout << EnvVarTable << std::endl << std::endl;
		std::cout << "HeapFunTable : " << std::endl;
		std::cout << HeapFunTable << std::endl << std::endl;
		std::cout << "HeapVarTable : " << std::endl;
		std::cout << HeapVarTable << std::endl << std::endl;
		//FIXME
	}

}