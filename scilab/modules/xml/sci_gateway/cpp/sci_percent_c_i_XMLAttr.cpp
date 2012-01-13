/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <algorithm>
#include <vector>

#include "XMLObject.hxx"
#include "XMLElement.hxx"
#include "XMLNs.hxx"
#include "XMLAttr.hxx"

extern "C"
{
#include <stdio.h>
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_percent_c_i_XMLAttr(char * fname, int* pvApiCtx)
{
    XMLAttr * a;
    int lhsid;
    SciErr err;
    int * prefixaddr = 0;
    int * nameaddr = 0;
    double * indexes = 0;
    int rows;
    int cols;
    int * rhsaddr = 0;
    int * lhsaddr = 0;
    char * name = 0;
    char * prefix = 0;
    char * value = 0;

    CheckLhs(1, 1);
    CheckRhs(3, 4);

    err = getVarAddressFromPosition(pvApiCtx, 1, &prefixaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (Rhs == 3 && isDoubleType(pvApiCtx, prefixaddr))
    {
        err = getMatrixOfDouble(pvApiCtx, prefixaddr, &rows, &cols, &indexes);
        if (rows != 1 || cols != 1)
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string or a single integer expected\n"), fname, 1);
            return 0;
        }
    }
    else
    {
        if (!isStringType(pvApiCtx, prefixaddr))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string or a single integer expected.\n"), fname, 1);
            return 0;
        }

        getAllocatedSingleString(pvApiCtx, prefixaddr, &prefix);

        if (Rhs == 4)
        {
            err = getVarAddressFromPosition(pvApiCtx, 2, &nameaddr);
            if (err.iErr)
            {
                freeAllocatedSingleString(prefix);
                printError(&err, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
                return 0;
            }

            if (!isStringType(pvApiCtx, nameaddr))
            {
                freeAllocatedSingleString(prefix);
                Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
                return 0;
            }

            getAllocatedSingleString(pvApiCtx, nameaddr, &name);
        }
    }

    err = getVarAddressFromPosition(pvApiCtx, Rhs - 1, &rhsaddr);
    if (err.iErr)
    {
        if (prefix)
        {
            freeAllocatedSingleString(prefix);
        }
        if (name)
        {
            freeAllocatedSingleString(name);
        }
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, Rhs, &lhsaddr);
    if (err.iErr)
    {
        if (prefix)
        {
            freeAllocatedSingleString(prefix);
        }
        if (name)
        {
            freeAllocatedSingleString(name);
        }
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, Rhs);
        return 0;
    }

    lhsid = getXMLObjectId(lhsaddr, pvApiCtx);
    a = XMLObject::getFromId<XMLAttr>(lhsid);
    if (!a)
    {
        if (prefix)
        {
            freeAllocatedSingleString(prefix);
        }
        if (name)
        {
            freeAllocatedSingleString(name);
        }
        Scierror(999, gettext("%s: XML object does not exist.\n"), fname);
        return 0;
    }

    if (!isStringType(pvApiCtx, rhsaddr))
    {
        if (prefix)
        {
            freeAllocatedSingleString(prefix);
        }
        if (name)
        {
            freeAllocatedSingleString(name);
        }
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, Rhs - 1);
        return 0;
    }

    getAllocatedSingleString(pvApiCtx, rhsaddr, &value);

    if (Rhs == 3)
    {
        if (indexes)
        {
            a->setAttributeValue((int)(*indexes), value);
        }
        else
        {
            name = prefix;
            a->setAttributeValue(name, value);
        }
    }
    else
    {
        a->setAttributeValue(prefix, name, value);
    }

    if (prefix)
    {
        freeAllocatedSingleString(prefix);
    }
    if (Rhs != 3 && name)
    {
        freeAllocatedSingleString(name);
    }
    freeAllocatedSingleString(value);

    a->createOnStack(Rhs + 1, pvApiCtx);
    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
