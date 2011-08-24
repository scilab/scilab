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
#include "XMLElement.hxx"

extern "C"
{
#include <stdio.h>
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "libxml/tree.h"
#include "expandPathVariable.h"
#include "MALLOC.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlWrite(char * fname, unsigned long fname_len)
{
    XMLDocument * doc = 0;
    xmlDoc * document = 0;
    SciErr err;
    int * addr = 0;
    char * path = 0;
    const char * expandedPath = 0;
    FILE * file = 0;
    int ret = 0;

    CheckLhs(1, 1);
    CheckRhs(1, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isXMLDoc(addr))
    {
        Scierror(999, gettext("%s: Wrong type for input argument %i: A %s expected.\n"), fname, 1, "XMLDoc");
        return 0;
    }

    doc = XMLObject::getFromId<XMLDocument>(getXMLObjectId(addr));
    if (!doc)
    {
        Scierror(999, gettext("%s: XML Document does not exist.\n"), fname);
        return 0;
    }
    document = doc->getRealDocument();

    if (Rhs == 2)
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%i: A string expected.\n"), fname, 2);
            return 0;
        }

        getAllocatedSingleString(pvApiCtx, addr, &path);

        if (!path || !strlen(path))
        {
            freeAllocatedSingleString(path);
            Scierror(999, gettext("%s: Wrong size for input argument #%d: Non-empty string expected.\n"), fname, 2);
            return 0;
        }

        expandedPath = const_cast<const char *>(expandPathVariable(path));
        freeAllocatedSingleString(path);
    }
    else
    {
        if (!document->URL)
        {
            Scierror(999, gettext("%s: The XML Document has not an URI and there is no second argument.\n"), fname);
            return 0;
        }
        expandedPath = strdup((const char *)document->URL);
    }

    file = fopen(expandedPath, "w");
    if (!file)
    {
        Scierror(999, gettext("%s: Cannot write the file: %s\n"), fname, expandedPath);
        FREE(expandedPath);
        return 0;
    }

    ret = xmlDocDump(file, document);
    if (ret == -1)
    {
        Scierror(999, gettext("%s: Cannot write the file: %s\n"), fname, expandedPath);
        FREE(expandedPath);
        return 0;
    }

    FREE(expandedPath);
    fclose(file);

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
