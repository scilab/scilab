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
#include "XMLElement.hxx"
#include "XMLNs.hxx"
#include "XMLAttr.hxx"
#include "XMLNodeList.hxx"

extern "C"
{
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "sciprint.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlIsValidObject(char *fname, void* pvApiCtx)
{
    int id;
    SciErr err;
    int *addr = 0;
    int row = 1;
    int col = 1;
    char **vars = 0;
    int *exists = 0;

    CheckLhs(1, 1);
    CheckRhs(1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, addr))
    {
        if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &vars) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
        exists = new int[row * col];

        for (int i = 0; i < row * col; i++)
        {
            err = getVarAddressFromName(pvApiCtx, const_cast < const char *>(vars[i]), &addr);

            if (err.iErr)
            {
                delete[] exists;
                printError(&err, 0);
                Scierror(999, _("%s: Can not read named argument %s.\n"), fname, const_cast < const char *>(vars[i]));
                freeAllocatedMatrixOfString(row, col, vars);

                return 0;
            }

            id = getXMLObjectId(addr, pvApiCtx);
            exists[i] = XMLObject::getFromId < XMLObject > (id) != 0;
        }

        freeAllocatedMatrixOfString(row, col, vars);
    }
    else
    {
        exists = new int[1];

        id = getXMLObjectId(addr, pvApiCtx);
        exists[0] = XMLObject::getFromId < XMLObject > (id) != 0;
    }

    err = createMatrixOfBoolean(pvApiCtx, Rhs + 1, row, col, exists);
    delete[]exists;
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
