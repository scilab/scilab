/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Vincent COUVERT
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

#include "function.hxx"
#include "action_binding_gw.hxx"
#include "string.hxx"
#include "Signal.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_notify(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pString  = NULL;

    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "notify" , 1);
        return types::Function::Error;
    }
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "notify", 1);
        return types::Function::Error;
    }
    pString = in[0]->getAs<types::String>();

    if (pString->isScalar() == FALSE)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), "notify" , 1);
        return types::Function::Error;
    }

    try
    {
        org_scilab_modules_action_binding_utils::Signal::notify(getScilabJavaVM(), pString->get(0));
    }
    catch (const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), "notify", e.whatStr().c_str());
        return types::Function::Error;
    }

    return types::Function::OK;
}

/*--------------------------------------------------------------------------*/
