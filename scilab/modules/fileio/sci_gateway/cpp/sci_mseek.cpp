/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Allan CORNET
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "bool.hxx"
#include "function.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "mseek.h"
}
/*--------------------------------------------------------------------------*/
#if (defined(sun) && !defined(SYSV))
char *strerror (int errcode);
#endif
/*--------------------------------------------------------------------------*/
#if (defined(sun) && !defined(SYSV)) || defined(sgi)
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_mseek(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iFile           = -1; //default file : last opened file
    int iRet            = 0;
    long long iWhere    = 0;
    int iFlag           = 0;
    wchar_t* wcsFlag    = NULL;

    if (in.size() < 1 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "mseek", 1, 3);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false || in[0]->getAs<types::Double>()->isScalar() == false || in[0]->getAs<types::Double>()->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A Real expected.\n"), "mseek", 1);
        return types::Function::Error;
    }

    iWhere = static_cast<long long>(in[0]->getAs<types::Double>()->get(0));

    if (in.size() == 2)
    {
        if (in[1]->isDouble() && in[1]->getAs<types::Double>()->isScalar() && in[1]->getAs<types::Double>()->isComplex() == false)
        {
            iFile = static_cast<int>(in[1]->getAs<types::Double>()->get(0));
        }
        else if (in[1]->isString() && in[1]->getAs<types::String>()->isScalar())
        {
            wcsFlag = in[1]->getAs<types::String>()->get(0);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real or String expected.\n"), "mseek", 2);
            return types::Function::Error;
        }
    }

    if (in.size() == 3)
    {
        if (in[1]->isDouble() == false || in[1]->getAs<types::Double>()->isScalar() == false || in[1]->getAs<types::Double>()->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "mseek", 2);
            return types::Function::Error;
        }
        if (in[2]->isString() == false || in[2]->getAs<types::String>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "mseek", 3);
            return types::Function::Error;
        }

        iFile = static_cast<int>(in[1]->getAs<types::Double>()->get(0));
        wcsFlag = in[2]->getAs<types::String>()->get(0);
    }

    switch (iFile)
    {
        case 0: // stderr
        case 5: // stdin
        case 6: // stdout
            Scierror(999, _("%s: Wrong file descriptor: %d.\n"), "mseek", iFile);
            return types::Function::Error;
    }

    if (wcsFlag != NULL)
    {
        if (wcsncmp(wcsFlag, L"set", 3) == 0)
        {
            iFlag = SEEK_SET;
        }
        else if (wcsncmp(wcsFlag, L"cur", 3) == 0)
        {
            iFlag = SEEK_CUR;
        }
        else if (wcsncmp(wcsFlag, L"end", 3) == 0)
        {
            iFlag = SEEK_END;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), "mseek", 3, "set", "cur", "end");
            return types::Function::Error;
        }
    }
    else
    {
        iFlag = SEEK_SET;
    }

    int iErr = mseek(iFile, iWhere, iFlag);

    out.push_back(new types::Bool(!iErr));
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
