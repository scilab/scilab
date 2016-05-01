/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
#include "funcmanager.hxx"
#include "core_gw.hxx"
#include "string.hxx"
#include "double.hxx"
#include "function.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "getversion.h"
#include "localization.h"
#include "Scierror.h"
#include "with_module.h"
#include "freeArrayOfString.h"
}
/*--------------------------------------------------------------------------*/
#define VERSION_STRING L"string_info"
/*--------------------------------------------------------------------------*/
static int getversion_no_rhs(char *fname, void* pvApiCtx);
static int getversion_one_rhs(char *fname, void* pvApiCtx);
static int getversion_two_rhs(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_getversion(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "getversion", 0, 2);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        if (_iRetCount != 1 && _iRetCount != 2)
        {
            Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "getveresion", 1, 2);
            return types::Function::Error;
        }

        wchar_t *pwstVer = getScilabVersionAsWideString();
        types::String* pOut1 = new types::String(pwstVer);
        out.push_back(pOut1);
        FREE(pwstVer);

        if (_iRetCount == 2)
        {
            int iOption = 0;
            wchar_t** pwstOption = getScilabVersionOptions(&iOption);
            types::String* pOut2 = new types::String(1, iOption);
            pOut2->set(pwstOption);
            out.push_back(pOut2);
            freeArrayOfWideString(pwstOption, iOption);
        }

    }
    else if (in.size() == 1)
    {
        if (in[0]->isString() == false || in[0]->getAs<types::String>()->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), "getversion", 1);
            return types::Function::Error;
        }

        if (_iRetCount != 1)
        {
            Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "getveresion", 1);
            return types::Function::Error;
        }

        wchar_t* pwstModule = in[0]->getAs<types::String>()->get()[0];
        if (with_module(pwstModule) || (wcscmp(pwstModule, L"scilab") == 0))
        {
            int versionSize = 0;
            int *version = getModuleVersion(pwstModule, &versionSize);
            if (version == NULL)
            {
                Scierror(999, _("%s: Wrong file version.xml %s.\n"), "getversion", pwstModule);
                return types::Function::Error;
            }

            types::Double* pOut = new types::Double(1, versionSize);
            pOut->setInt(version);
            out.push_back(pOut);
            FREE(version);
        }
    }
    else //in.size() == 2
    {
        if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), "getversion", 1);
            return types::Function::Error;
        }

        if (in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: String expected.\n"), "getversion", 2);
            return types::Function::Error;
        }

        wchar_t* pwstModule = in[0]->getAs<types::String>()->get()[0];
        wchar_t* pwstOption = in[1]->getAs<types::String>()->get()[0];

        if ( with_module(pwstModule) || (wcscmp(pwstModule, L"scilab") == 0) )
        {
            if ( wcscmp(pwstOption, VERSION_STRING) == 0)
            {
                wchar_t *pwstInfo = getModuleVersionInfoAsString(pwstModule);
                types::String* pOut = new types::String(pwstInfo);
                out.push_back(pOut);
                FREE(pwstInfo);
            }
        }
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
