/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
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
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_percent_c_i_XMLAttr(char *fname, void* pvApiCtx)
{
    XMLAttr *a;
    int lhsid;
    SciErr err;
    int *prefixaddr = 0;
    int *nameaddr = 0;
    double *indexes = 0;
    int rows;
    int cols;
    int *rhsaddr = 0;
    int *lhsaddr = 0;
    char *name = 0;
    char *prefix = 0;
    char *value = 0;

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
            Scierror(999, gettext("%s: Wrong type for input argument #%d: string or single integer expected\n"), fname, 1);
            return 0;
        }
    }
    else
    {
        if (!isStringType(pvApiCtx, prefixaddr) || !checkVarDimension(pvApiCtx, prefixaddr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: string or single integer expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, prefixaddr, &prefix) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

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

            if (!isStringType(pvApiCtx, nameaddr) || !checkVarDimension(pvApiCtx, nameaddr, 1, 1))
            {
                freeAllocatedSingleString(prefix);
                Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
                return 0;
            }

            if (getAllocatedSingleString(pvApiCtx, nameaddr, &name) != 0)
            {
                Scierror(999, _("%s: No more memory.\n"), fname);
                freeAllocatedSingleString(prefix);
                return 0;
            }
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
    a = XMLObject::getFromId < XMLAttr > (lhsid);
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

    if (!isStringType(pvApiCtx, rhsaddr) || !checkVarDimension(pvApiCtx, rhsaddr, 1, 1))
    {
        if (prefix)
        {
            freeAllocatedSingleString(prefix);
        }
        if (name)
        {
            freeAllocatedSingleString(name);
        }
        Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, Rhs - 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, rhsaddr, &value) != 0)
    {
        if (prefix)
        {
            freeAllocatedSingleString(prefix);
        }
        if (name)
        {
            freeAllocatedSingleString(name);
        }
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

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
