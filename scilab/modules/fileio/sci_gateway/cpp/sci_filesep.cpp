/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#include "function.hxx"
#include "string.hxx"
#include "funcmanager.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"

extern "C"
{
    /* DIR_SEPARATORW */
#include "machine.h"
#include "Scierror.h"
#include "localization.h"
#include "os_string.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_filesep(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "filesep" , 0);
        return types::Function::Error;
    }

    if (_iRetCount != 1 && _iRetCount != -1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "filesep", 1);
        return types::Function::Error;
    }

    types::String* pS = new types::String(DIR_SEPARATORW);
    out.push_back(pS);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

