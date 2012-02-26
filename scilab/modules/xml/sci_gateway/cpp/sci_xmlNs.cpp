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
#include "XMLNs.hxx"
#include "XMLElement.hxx"

extern "C"
{
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlNs(char *fname, unsigned long fname_len)
{
    int *addr = 0;
    SciErr err;
    XMLNs *ns = 0;
    XMLElement *elem = 0;
    char *prefix = 0;
    char *href = 0;
    int i = 0;
    char **vars[] = { &prefix, &href };

    CheckLhs(1, 1);
    CheckRhs(3, 3);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isXMLElem(addr, pvApiCtx))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A %s expected.\n"), fname, 1, "XMLElem");
        return 0;
    }

    elem = XMLObject::getFromId < XMLElement > (getXMLObjectId(addr, pvApiCtx));
    if (!elem)
    {
        Scierror(999, gettext("%s: XML Element does not exist.\n"), fname);
        return 0;
    }

    for (; i < Rhs - 1; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i + 2, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i + 2);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, i + 2);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, vars[i]) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    ns = new XMLNs(*elem, prefix, href);

    for (i = 0; i < Rhs - 1; i++)
    {
        freeAllocatedSingleString(*(vars[i]));
    }

    if (!ns->createOnStack(Rhs + 1, pvApiCtx))
    {
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
