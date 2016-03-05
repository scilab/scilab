/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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

#include "core_gw.hxx"
#include "configvariable.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_mode(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::InternalType *pIT = NULL;

    /* Check the number of input argument */
    if (in.size() > 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "mode" , 0, 1);
        return types::Function::Error;
    }

    /* Check the number of output argument */
    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "mode", 1);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        out.push_back(new types::Double(ConfigVariable::getPromptMode()));
    }
    else
    {
        if (in[0]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Scalar expected.\n"), "mode", 1);
            return types::Function::Error;
        }

        types::Double* pDblIn = in[0]->getAs<types::Double>();

        if (pDblIn->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "mode", 1);
            return types::Function::Error;
        }

        int iScilabMode = (int)pDblIn->get(0);
        if (pDblIn->get(0) != (double)iScilabMode)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: An integer value expected.\n"), "mode", 1);
            return types::Function::Error;
        }

        ConfigVariable::setPromptMode(iScilabMode);

        if (ConfigVariable::isPrintInteractive())
        {
            sciprint(_("Pause mode: enter empty lines to continue.\n"));
        }
    }

    return types::Function::OK;
}
