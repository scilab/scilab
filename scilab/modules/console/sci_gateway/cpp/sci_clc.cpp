/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "console_gw.hxx"
#include "function.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "clc.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_clc(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	BOOL res = FALSE;
	int nblines = -1;

    if(in.size() == 0)
    {
		/* Clear console full */
		nblines = -1;
    }
    else if(in.size() == 1)
    {
        if((in[0]->isDouble() == false) || !in[0]->getAs<types::Double>()->isScalar())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A double scalar expected.\n"), "clc", 1);
            return types::Function::Error;
        }
        nblines = (int)in[0]->getAs<types::Double>()->get(0);
        if(nblines < 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: A positive double expected\n"), "clc", 1);
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "clc", 0, 1);
        return types::Function::Error;
    }

    res = clc(nblines);
    if (!res)
    {
        Scierror(999, _("%s: This feature has not been implemented in this mode.\n"), "clc");
        return types::Function::Error;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
