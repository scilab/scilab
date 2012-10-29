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
#include "XMLList.hxx"

extern "C"
{
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
#include "stringToDouble.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlAsNumber(char *fname, unsigned long fname_len)
{
    int id;
    SciErr err;
    int *addr = 0;
    XMLList *list = 0;
    double *pdblReal = 0;

    CheckLhs(1, 1);
    CheckRhs(1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isXMLList(addr, pvApiCtx) && !isXMLSet(addr, pvApiCtx))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: XMLSet or XMLList expected.\n"), fname, 1);
        return 0;
    }

    id = getXMLObjectId(addr, pvApiCtx);
    list = XMLObject::getFromId < XMLList > (id);
    if (!list)
    {
        Scierror(999, gettext("%s: XMLSet or XMLList does not exist.\n"), fname);
        return 0;
    }

    if (list->getSize())
    {
        err = allocMatrixOfDouble(pvApiCtx, Rhs + 1, 1, list->getSize(), &pdblReal);
        const char **contents = list->getContentFromList();

        for (int i = 0; i < list->getSize(); i++)
        {
            stringToDoubleError convErr = STRINGTODOUBLE_NO_ERROR;

            pdblReal[i] = stringToDouble(contents[i], TRUE, &convErr);
            xmlFree(const_cast < char *>(contents[i]));
        }

        delete[]contents;
    }
    else
    {
        createEmptyMatrix(pvApiCtx, Rhs + 1);
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
