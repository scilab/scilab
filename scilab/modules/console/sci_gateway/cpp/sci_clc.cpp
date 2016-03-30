/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

    if (in.size() == 0)
    {
        /* Clear console full */
        nblines = -1;
    }
    else if (in.size() == 1)
    {
        if ((in[0]->isDouble() == false) || !in[0]->getAs<types::Double>()->isScalar())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A double scalar expected.\n"), "clc", 1);
            return types::Function::Error;
        }
        nblines = (int)in[0]->getAs<types::Double>()->get(0);
        if (nblines < 0)
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
