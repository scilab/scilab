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

#include "XMLObject.hxx"
#include "XMLDocument.hxx"

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
int sci_xmlDocument(char *fname, void* pvApiCtx)
{
    int *addr = 0;
    SciErr err;

    org_modules_xml::XMLDocument * doc = 0;
    char *uri = 0;
    char *version = 0;
    int i = 0;
    char **vars[] = { &uri, &version };

    CheckLhs(1, 1);
    CheckRhs(0, 2);

    for (; i < Rhs; i++)
    {
        err = getVarAddressFromPosition(pvApiCtx, i + 1, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, i + 1);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, i + 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, addr, vars[i]) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    doc = new org_modules_xml::XMLDocument(uri, version);

    for (i = 0; i < Rhs; i++)
    {
        freeAllocatedSingleString(*(vars[i]));
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
