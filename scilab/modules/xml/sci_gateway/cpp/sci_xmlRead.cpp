/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "xml.h"

extern "C"
{
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

#include "XMLObject.hxx"
#include "XMLDocument.hxx"

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlRead(char *fname, unsigned long fname_len)
{
    org_modules_xml::XMLDocument * doc;
    SciErr err;
    int *addr = 0;
    char *path = 0;
    char *encoding = 0;

    std::string error;
    bool validate = false;
    int validateParam;

    CheckLhs(1, 1);
    CheckRhs(1, 3);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &path) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    if (Rhs >= 2)
    {
        int validatePos = 2;

        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            freeAllocatedSingleString(path);
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (!checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            freeAllocatedSingleString(path);
            Scierror(999, gettext("%s: Wrong dimensions for input argument #%d: A single boolean or string expected.\n"), fname, 2);
            return 0;
        }

        if (isStringType(pvApiCtx, addr))
        {
            if (getAllocatedSingleString(pvApiCtx, addr, &encoding) != 0)
            {
                freeAllocatedSingleString(path);
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }

            if (Rhs == 3)
            {
                err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
                if (err.iErr)
                {
                    freeAllocatedSingleString(path);
                    freeAllocatedSingleString(encoding);
                    printError(&err, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
                    return 0;
                }
                validatePos = 3;
            }
        }

        if (encoding == 0 && Rhs == 3)
        {
            freeAllocatedSingleString(path);
            Scierror(999, _("%s: Invalid number of input arguments.\n"), fname);
            return 0;
        }

        if (encoding == 0 || Rhs == 3)
        {
            if (!isBooleanType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
            {
                freeAllocatedSingleString(path);
                if (encoding)
                {
                    freeAllocatedSingleString(encoding);
                }
                Scierror(999, gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, validatePos);
                return 0;
            }

            getScalarBoolean(pvApiCtx, addr, &validateParam);
            validate = validateParam != 0;
        }
    }

    doc = new org_modules_xml::XMLDocument((const char *)path, validate, &error, encoding);
    freeAllocatedSingleString(path);
    if (encoding)
    {
        freeAllocatedSingleString(encoding);
    }

    if (!error.empty())
    {
        delete doc;

        Scierror(999, gettext("%s: Cannot read the file:\n%s"), fname, error.c_str());
        return 0;
    }

    if (!doc->createOnStack(Rhs + 1, pvApiCtx))
    {
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
