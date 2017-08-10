/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - 2011 - DIGITEO - Bruno JOFRET
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
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include "fullpath.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_fullpath(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input arguments: %d expected.\n"), "fullpath" , 1);
        return types::Function::Error;
    }

    if (_iRetCount != 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "fullpath", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() && in[0]->getAs<types::Double>()->isEmpty())
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "fullpath", 1);
        return types::Function::Error;
    }

    char fullpath[PATH_MAX * 4];
    types::String* pIn = in[0]->getAs<types::String>();
    types::String* pOut = new types::String(pIn->getDims(), pIn->getDimsArray());

    for (int i = 0 ; i < pIn->getSize() ; i++)
    {
        char *relPath = wide_string_to_UTF8(pIn->get(i));
        if ( get_full_path(fullpath, relPath, PATH_MAX * 4 ) != NULL)
        {
            pOut->set(i, fullpath);
            FREE(relPath);
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' is an invalid path.\n"), "fullpath", 1, relPath);
            FREE(relPath);
            return types::Function::Error;
        }

        fullpath[0] = L'\0';
    }

    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
