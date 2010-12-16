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
#include "funcmanager.hxx"
#include "context.hxx"
#include "configvariable.hxx"

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
	wstring stModule;

	/* Check the number of input argument */
	if(in.size() > 1)
	{
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d or %d expected.\n"), L"mode" , 0, 1);
		return Function::Error;
	}

	/* Check the number of output argument */
	if(_iRetCount != 1)
	{
		ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"mode", 1);
        return Function::Error;
	}

    if(in.size() == 0)
    {
        out.push_back(new Double(ConfigVariable::getPromptMode()));
    }
    else
    {
        if(in[0]->isDouble() == false || in[0]->getAsDouble()->size_get() != 1)
	    {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Scalar expected.\n"), L"mode", 1);
            return Function::Error;
        }

        ConfigVariable::setPromptMode((int)in[0]->getAsDouble()->real_get()[0]);

    }

	return Function::OK;
}
/*--------------------------------------------------------------------------*/
