/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "timedvisitor.hxx"
#include "timer.hxx"
#include "localization.h"

#include "yaspio.hxx"

using std::string;

namespace ast
{
	void TimedVisitor::visit(const CallExp &e)
	{
		std::cout << "TIMED !!!" << std::endl;
	}
}
