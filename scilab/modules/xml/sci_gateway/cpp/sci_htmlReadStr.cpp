/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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
#include "XMLRhsValue.hxx"

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
int sci_htmlReadStr(char *fname, void* pvApiCtx)
{
    org_modules_xml::XMLDocument * doc;
    SciErr err;
    int *addr = 0;
    char *encoding = 0;

    std::string * code;
    std::string error;

    CheckLhs(1, 2);
    CheckRhs(1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 0;
    }

    if (!XMLRhsValue::get(fname, addr, &code, pvApiCtx))
    {
        return 0;
    }

    doc = new org_modules_xml::XMLDocument(*code, false, &error, 0, true);
    delete code;

    if (!error.empty())
    {
        delete doc;

        Scierror(999, gettext("%s: Cannot parse the string:\n%s"), fname, error.c_str());
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
