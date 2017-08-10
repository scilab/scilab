/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
/*
 * Provides is_dir to scilab
 *
 */
/*--------------------------------------------------------------------------*/
#include "function.hxx"
#include "string.hxx"
#include "fileio_gw.hxx"
#include "bool.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "isdir.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_isdir(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "isdir" , 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "isdir", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "isdir", 1);
        return types::Function::Error;
    }

    types::String* pS = in[0]->getAs<types::String>();
    types::Bool* pOut = new types::Bool(pS->getRows(), pS->getCols());

    for (int i = 0 ; i < pS->getSize() ; i++)
    {
        wchar_t *expandedPath = expandPathVariableW(pS->get(i));
        if (expandedPath)
        {
            pOut->get()[i] = isdirW(expandedPath);
            FREE(expandedPath);
            expandedPath = NULL;
        }
        else
        {
            pOut->get()[i] = FALSE;
        }
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
