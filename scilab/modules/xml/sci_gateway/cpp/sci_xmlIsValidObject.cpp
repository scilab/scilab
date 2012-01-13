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
#include "XMLNs.hxx"
#include "XMLAttr.hxx"
#include "XMLNodeList.hxx"

extern "C"
{
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "sciprint.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlIsValidObject(char * fname, int *_piKey)
{
    XMLObject * obj = 0;
    int id;
    SciErr err;
    int * addr = 0;
    int row = 1;
    int col = 1;
    char ** vars = 0;
    int * exists = 0;

    CheckLhs(1, 1);
    CheckRhs(1, 1);

    err = getVarAddressFromPosition(_piKey, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(_piKey, addr))
    {
        getAllocatedMatrixOfString(_piKey, addr, &row, &col, &vars);
        exists = new int[row * col];
        for (int i = 0; i < row * col; i++)
        {
            err = getVarAddressFromName(_piKey, const_cast<const char *>(vars[i]), &addr);
            if (err.iErr)
            {
                delete[] exists;
                freeAllocatedMatrixOfString(row, col, vars);
                printError(&err, 0);
                Scierror(999, _("%s: Can not read named argument %s.\n"), fname, const_cast<const char *>(vars[i]));
                return 0;
            }

            id = getXMLObjectId(addr, _piKey);
            exists[i] = XMLObject::getFromId<XMLObject>(id) != 0;
        }

        freeAllocatedMatrixOfString(row, col, vars);
    }
    else
    {
        exists = new int[1];
        id = getXMLObjectId(addr, _piKey);
        exists[0] = XMLObject::getFromId<XMLObject>(id) != 0;
    }

    err = createMatrixOfBoolean(_piKey, Rhs + 1, row, col, exists);
    delete[] exists;
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999,_("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
