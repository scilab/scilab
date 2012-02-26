/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "XMLObject.hxx"
#include "XMLAttr.hxx"

extern "C"
{
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
#include <string.h>
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_percent_XMLAttr_size(char *fname, unsigned long fname_len)
{
    int id;
    SciErr err;
    double d[2] = { 1, 0 };
    int * addr = 0;
    XMLAttr * attrs;
    int size;
    char * option = 0;

    CheckLhs(1, 1);
    CheckRhs(1, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isXMLAttr(addr, pvApiCtx))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A %s expected.\n"), fname, 1, "XMLAttr");
        return 0;

    }

    id = getXMLObjectId(addr, pvApiCtx);
    attrs = XMLObject::getFromId<XMLAttr>(id);
    if (!attrs)
    {
        Scierror(999, gettext("%s: XMLList does not exist.\n"), fname);
        return 0;
    }

    if (Rhs == 2)
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 2);
            return 0;
        }
        if (getAllocatedSingleString(pvApiCtx, addr, &option) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        if (strcmp(option, "r") && strcmp(option, "c") && strcmp(option, "*"))
        {
            freeAllocatedSingleString(option);
            Scierror(44, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), fname, 2, "r", "c", "*");
            return 0;
        }
    }

    size = (int)(attrs->getSize());
    if (Rhs == 1)
    {
        d[1] = size;
        if (size == 0)
        {
            d[0] = 0;
        }
        err = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 2, d);
    }
    else
    {
        if (option[0] == 'r')
        {
            if (size == 0)
            {
                d[0] = 0;
            }
        }
        else
        {
            d[0] = size;
        }
        freeAllocatedSingleString(option);
        err = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, d);
    }

    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
