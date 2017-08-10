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
#include "XMLNodeList.hxx"

extern "C"
{
#include <string.h>
#include <stdio.h>
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

int sci_percent_XMLList_e(char *fname, void* pvApiCtx)
{
    XMLList *list = 0;
    const XMLObject *elem;
    int id;
    SciErr err;
    double *dvalue = 0;
    int *mlistaddr = 0;
    int *daddr = 0;
    int typ = 0;
    int row;
    int col;
    int index;
    double d;
    char *field = 0;
    const char **pstStrings = 0;

    CheckLhs(1, 1);
    CheckRhs(2, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &daddr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    err = getVarDimension(pvApiCtx, daddr, &row, &col);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    err = getVarType(pvApiCtx, daddr, &typ);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (typ == sci_strings)
    {
        if (row != 1 || col != 1)
        {
            Scierror(999, gettext("%s: Wrong dimension for input argument #%d: string or double expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, daddr, &field) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
        err = getVarAddressFromPosition(pvApiCtx, 2, &mlistaddr);
        if (err.iErr)
        {
            freeAllocatedSingleString(field);
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        id = getXMLObjectId(mlistaddr, pvApiCtx);
        list = XMLObject::getFromId < XMLList > (id);
        if (!list)
        {
            freeAllocatedSingleString(field);
            Scierror(999, gettext("%s: XML object does not exist.\n"), fname);
            return 0;
        }

        if (!strcmp(field, "size"))
        {
            d = (double)list->getSize();
            createScalarDouble(pvApiCtx, Rhs + 1, d);

            LhsVar(1) = Rhs + 1;
            PutLhsVar();
        }
        else if (!strcmp(field, "content"))
        {
            pstStrings = list->getContentFromList();

            err = createMatrixOfString(pvApiCtx, Rhs + 1, 1, list->getSize(), const_cast < const char * const *>(pstStrings));

            // contents are created with xmlGetNodeContent which requires that the user free the himself the memory
            for (int i = 0; i < list->getSize(); i++)
            {
                xmlFree(const_cast < char *>(pstStrings[i]));
            }
            delete[]pstStrings;
            if (err.iErr)
            {
                printError(&err, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(1) = Rhs + 1;
            PutLhsVar();
        }
        else if (!strcmp(field, "name"))
        {
            pstStrings = list->getNameFromList();

            err = createMatrixOfString(pvApiCtx, Rhs + 1, 1, list->getSize(), const_cast < const char * const *>(pstStrings));

            delete[]pstStrings;
            if (err.iErr)
            {
                printError(&err, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }

            LhsVar(1) = Rhs + 1;
            PutLhsVar();
        }
        else
        {
            Scierror(999, gettext("%s: Unknown field: %s\n"), fname, field);
        }
        freeAllocatedSingleString(field);

        return 0;
    }

    if (row != 1 || col != 1 || typ != sci_matrix)
    {
        Scierror(999, gettext("%s: Wrong dimension for input argument #%d: Single double expected.\n"), fname, 1);
        return 0;
    }

    if (isVarComplex(pvApiCtx, daddr))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: Double expected.\n"), fname, 1);
        return 0;
    }

    err = getMatrixOfDouble(pvApiCtx, daddr, &row, &col, &dvalue);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &mlistaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    id = getXMLObjectId(mlistaddr, pvApiCtx);
    list = XMLObject::getFromId < XMLList > (id);
    if (!list)
    {
        Scierror(999, gettext("%s: XML object does not exist.\n"), fname);
        return 0;
    }

    index = (int)(*dvalue);
    elem = list->getListElement(index);
    if (!elem)
    {
        Scierror(999, gettext("%s: Wrong index in the XMLList.\n"), fname);
        return 0;
    }

    if (!elem->createOnStack(Rhs + 1, pvApiCtx))
    {
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
