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
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlClose(char * fname, unsigned long fname_len)
{
    int id;
    SciErr err;
    int * addr = 0;
    org_modules_xml::XMLDocument * doc;

    CheckLhs(1, 1);
    CheckRhs(1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
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
            Scierror(999, "%s: XML document does not exist\n", fname);
            return 0;
        }
        delete doc;
    }
    else
    {
        Scierror(999, "%s: Wrong type for input argument %i: %s expected\n", fname, 1, "XMLDoc");
        return 0;
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
