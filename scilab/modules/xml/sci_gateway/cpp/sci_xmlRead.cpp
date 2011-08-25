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

#include <iostream>

extern "C"
{
#include "xml.h"
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
}

#include "XMLObject.hxx"
#include "XMLDocument.hxx"


using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlRead(char *fname, unsigned long fname_len)
{
    org_modules_xml::XMLDocument *doc;
    
    SciErr err;
    int *addr = 0;
    char *path = 0;
    char *error = 0;
    
    CheckLhs(1, 1);
    CheckRhs(1, 1);
   
    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr))
    {
        Scierror(999, "%s: Wrong type for input argument %i: String expected\n", fname, 1);
        return 0;
    }
    
    getAllocatedSingleString(pvApiCtx, addr, &path);
    
    doc = new org_modules_xml::XMLDocument((const char *) path, &error);
    freeAllocatedSingleString(path);

    if (error)
    {
	delete doc;
	Scierror(999, "%s: Cannot read the file:\n%s", fname, error);
        return 0;
    }

    if (!doc->createOnStack(Rhs + 1))
    {
	return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
