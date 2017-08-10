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
#include "XMLElement.hxx"
#include "XMLNodeList.hxx"

extern "C"
{
#include <stdio.h>
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_percent_s_i_XMLList(char *fname, void* pvApiCtx)
{
    XMLNodeList *a;
    int lhsid;
    int row, col;
    double index;
    SciErr err;
    int *indexaddr = 0;
    int *rhsaddr = 0;
    int *lhsaddr = 0;
    int *retaddr = 0;
    char *retstr = 0;
    int iBegin = 2;
    int mrhs = 1;
    int mlhs = 1;

    CheckLhs(1, 1);
    CheckRhs(3, 3);

    err = getVarAddressFromPosition(pvApiCtx, 1, &indexaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isDoubleType(pvApiCtx, indexaddr) || !checkVarDimension(pvApiCtx, indexaddr, 1, 1))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A double expected.\n"), fname, 1);
        return 0;
    }

    getScalarDouble(pvApiCtx, indexaddr, &index);

    err = getVarAddressFromPosition(pvApiCtx, 2, &rhsaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 3, &lhsaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        return 0;
    }

    lhsid = getXMLObjectId(lhsaddr, pvApiCtx);
    a = XMLObject::getFromId < XMLNodeList > (lhsid);
    if (!a)
    {
        Scierror(999, gettext("%s: XML object does not exist.\n"), fname);
        return 0;
    }

    err = getMatrixOfDouble(pvApiCtx, rhsaddr, &row, &col, 0);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (row == 0 && col == 0)
    {
        a->removeElementAtPosition((int)index);
        a->createOnStack(Rhs + 1, pvApiCtx);
        LhsVar(1) = Rhs + 1;
    }
    else if (isNamedVarExist(pvApiCtx, "%s_xmlFormat"))
    {
        callScilabFunction(pvApiCtx, "%s_xmlFormat", iBegin, mlhs, mrhs);
        //Call function directly in scilab 6 C++ api
        //Nbvars = std::max(Nbvars, Lhs + Rhs);
        //SciString(&iBegin, (char *)"%s_xmlFormat", &mlhs, &mrhs);
        //Nbvars = std::max(Nbvars, Lhs + Rhs + mlhs + mrhs);

        iBegin = nbInputArgument(pvApiCtx) + mrhs + 1;
        err = getVarAddressFromPosition(pvApiCtx, iBegin, &retaddr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iBegin);
            return 0;
        }

        if (!isStringType(pvApiCtx, retaddr) || !checkVarDimension(pvApiCtx, retaddr, 1, 1))
        {
            Scierror(999, gettext("%s: xmlFormat must return a string.\n"), fname);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, retaddr, &retstr) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
        if (retstr)
        {
            a->setElementAtPosition(index, std::string(retstr));
            freeAllocatedSingleString(retstr);
            a->createOnStack(Rhs + 1, pvApiCtx);
            LhsVar(1) = Rhs + 1;
        }
    }
    else
    {
        Scierror(999, gettext("%s: You must define %%s_xmlFormat function.\n"), fname);
        return 0;
    }

    PutLhsVar();

    return 0;
}
