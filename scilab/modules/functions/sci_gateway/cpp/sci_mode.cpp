/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "functions_gw.hxx"
#include "configvariable.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_mode(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	InternalType *pIT = NULL;
    std::wstring stModule;

	/* Check the number of input argument */
	if(in.size() > 1)
	{
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "mode" , 0, 1);
		return Function::Error;
	}

	/* Check the number of output argument */
	if(_iRetCount != 1)
	{
		Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "mode", 1);
        return Function::Error;
	}

    if(in.size() == 0)
    {
        out.push_back(new Double(ConfigVariable::getPromptMode()));
    }
    else
    {
        if(in[0]->isDouble() == false || in[0]->getAs<Double>()->getSize() != 1)
	    {
            Scierror(999, _("%s: Wrong type for input argument #%d: Scalar expected.\n"), "mode", 1);
            return Function::Error;
        }

        ConfigVariable::setPromptMode((int)in[0]->getAs<Double>()->getReal()[0]);

    }

	return Function::OK;
}
/*--------------------------------------------------------------------------*/
