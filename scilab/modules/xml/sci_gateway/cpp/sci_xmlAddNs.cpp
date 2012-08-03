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
#include "XMLElement.hxx"
#include "XMLNs.hxx"

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
int sci_xmlAddNs(char *fname, unsigned long fname_len)
{
    XMLElement *elem = 0;
    XMLNs *ns = 0;
    SciErr err;
    int *addr = 0;
    int i = 2;

    CheckLhs(1, 1);
    if (Rhs < 2)
    {
        Scierror(999, gettext("%s: Wrong number of input arguments: at least %d expected.\n"), fname, 2);
        return 0;
    }

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

    for (; i <= Rhs; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i);
            return 0;
        }

        if (!isXMLNs(addr, pvApiCtx))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A %s expected.\n"), fname, i, "XMLNs");
            return 0;
        }

        ns = XMLObject::getFromId < XMLNs > (getXMLObjectId(addr, pvApiCtx));
        if (!ns)
        {
            Scierror(999, gettext("%s: XML Namespace does not exist.\n"), fname);
            return 0;
        }

        elem->addNamespace(*ns);
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
