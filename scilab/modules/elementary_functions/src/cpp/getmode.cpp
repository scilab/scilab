/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Antoine ELIAS
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

#include "getmode.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}


int getMode(types::typed_list &in, int _iProcess, int _iRef)
{
    int iMode = 0;
    if (in[_iProcess]->isString())
    {
        types::String* pS = in[_iProcess]->getAs<types::String>();
        if (pS->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "size", _iProcess + 1, 1, 1);
        }

        switch (pS->get(0)[0])
        {
            case 'r' :
                iMode = 1;
                break;
            case 'c' :
                iMode = 2;
                break;
            case '*' :
                iMode = 0;
                break;
            case 'm' :
                iMode = -1;
                break;
            default :
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"), "size", _iProcess + 1, "m" , "*" , "r", "c");
                iMode = -2;
                break;
        }
    }
    else if (in[1]->isDouble() && in[1]->getAs<types::Double>()->isComplex() == false)
    {
        types::Double* pD = in[_iProcess]->getAs<types::Double>();
        if (pD->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "size", _iProcess + 1, 1, 1);
            iMode = -2;
        }
        else
        {
            iMode = static_cast<int>(pD->getReal()[0]);
            if (pD->getReal()[0] != static_cast<double>(iMode))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: An integer value expected.\n"), "size", 2);
                iMode = -2;
            }

            if (iMode <= 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), "size", 2);
                iMode = -2;
            }
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string or scalar expected.\n"), "size", 2);
        iMode = -2;
    }

    //special case for -1
    if (iMode == -1)
    {
        iMode = 0;
        if (in[_iRef]->getAs<types::GenericType>()->getRows() > 1)
        {
            iMode = 1;
        }
        else if (in[_iRef]->getAs<types::GenericType>()->getCols() > 1)
        {
            iMode = 2;
        }
    }

    return iMode;
}

