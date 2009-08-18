/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
* 
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
* 
*/

#include "macro.hxx"

namespace types
{

	/*--------------*/
	/*	Contructor  */
	/*--------------*/
	Macro::Macro(std::string _stName, std::list<symbol::Symbol> &_inputArgs, std::list<symbol::Symbol> &_outputArgs, ast::SeqExp &_body)
	{
		m_stName			= _stName;
		m_inputArgs		= &_inputArgs;
		m_outputArgs	= &_outputArgs;
		m_body				= &_body;
	}

	/*--------------*/
	/*	whoIAm		  */
	/*--------------*/
	void Macro::whoAmI()
	{
		std::cout << "types::Macro";
	}

	Macro* Macro::getAsMacro(void)
	{
		return this; 
	}

	InternalType::RealType Macro::getType(void)
	{ 
		return RealMacro; 
	}

}
