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

#include <cstdlib>

#include "XMLObject.hxx"
#include "XMLNodeSet.hxx"

extern "C"
{
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
#include "stringToDouble.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlAsNumber(char * fname, unsigned long fname_len)
{
    int id;
    SciErr err;
    int * addr = 0;
    XMLNodeSet * set = 0;
    xmlNodeSet * realSet = 0;
    double * pdblReal = 0;

    CheckLhs(1, 1);
    CheckRhs(1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isXMLSet(addr, pvApiCtx))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: XMLSet expected.\n"), fname, 1);
        return 0;

    }

    id = getXMLObjectId(addr, pvApiCtx);
    set = XMLObject::getFromId<XMLNodeSet>(id);
    if (!set)
    {
        Scierror(999, gettext("%s: XMLSet does not exist.\n"), fname);
        return 0;
    }

    err = allocMatrixOfDouble(pvApiCtx, Rhs + 1, 1, set->getSize(), &pdblReal);
    realSet = static_cast<xmlNodeSet *>(set->getRealXMLPointer());

    for (int i = 0; i < set->getSize(); i++)
    {
        xmlNode * node = realSet->nodeTab[i];
        const char * content = (const char *)xmlNodeGetContent(node);
        stringToDoubleError convErr = STRINGTODOUBLE_NO_ERROR;
        pdblReal[i] = stringToDouble(content, TRUE, &convErr);
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
