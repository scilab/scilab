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

#include "XMLObject.hxx"
#include "XMLDocument.hxx"

extern "C"
{
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlClose(char * fname, unsigned long fname_len)
{
    int id;
    SciErr err;
    int * addr = 0;
    org_modules_xml::XMLDocument * doc;
    char * com = 0;

    CheckLhs(1, 1);

    if (Rhs == 0)
    {
        Scierror(999, gettext("%s: Wrong number of input arguments: at least %d expected.\n"), fname, 1);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (isStringType(pvApiCtx, addr))
    {
        getAllocatedSingleString(pvApiCtx, addr, &com);
        if (!strcmp(com, "all"))
        {
            org_modules_xml::XMLDocument::closeAllDocuments();
        }
        freeAllocatedSingleString(com);
    }
    else
    {
        for (int pos = 1; pos <= Rhs; pos++)
        {
            err = getVarAddressFromPosition(pvApiCtx, pos, &addr);
            if (err.iErr)
            {
                printError(&err, 0);
                return 0;
            }

            if (isXMLDoc(addr))
            {
                id = getXMLObjectId(addr);
                doc = XMLObject::getFromId<org_modules_xml::XMLDocument>(id);
                if (!doc)
                {
                    Scierror(999, gettext("%s: XML document does not exist.\n"), fname);
                    return 0;
                }
                delete doc;
            }
            else
            {
                Scierror(999, gettext("%s: Wrong type for input argument #%i: A %s expected\n"), fname, 1, "XMLDoc");
                return 0;
            }
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
