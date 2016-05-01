/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
 *
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
#include "string.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "filemanager.hxx"
#include "function.hxx"

extern "C"
{
#include <stdio.h>
#include "expandPathVariable.h"
#include "configvariable_interface.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_meof(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRet  = 0;
    int iFile = -1; //default file : last opened file

    if (in.size() > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "meof", 0, 1);
        return types::Function::Error;
    }
    if (in.size() == 1)
    {
        if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "meof", 1);
            return types::Function::Error;
        }

        iFile = (int)in[0]->getAs<types::Double>()->get(0);
        switch (iFile)
        {
            case 0: // stderr
            case 5: // stdin
            case 6: // stdout
                Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "meof", iFile);
                return types::Function::Error;
        }
    }

    types::File* pF = FileManager::getFile(iFile);
    if (pF != NULL)
    {
        iRet = feof(pF->getFiledesc());
    }
    else
    {
        if (getWarningMode())
        {
            sciprint(_("%ls: Cannot check the end of file whose descriptor is %d: File is not active.\n"), "meof", iFile);
        }
        return types::Function::OK;
    }

    types::Double* pOut = new types::Double(iRet);
    out.push_back(pOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
