/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __XMLLIST_INSERT_HPP__
#define __XMLLIST_INSERT_HPP__

#include "XMLRhsValue.hxx"

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

/**
 * Function to handle insertion in XMLNodeList
 * @param fname the function name
 * @param fname_len the function name length
 */
template <class T>
int sci_XMLList_insertion(char * fname, void* pvApiCtx)
{
    XMLNodeList * a;
    T * b;
    int lhsid;
    bool success;
    double index;
    SciErr err;
    int * indexaddr = 0;
    int * rhsaddr = 0;
    int * lhsaddr = 0;

    CheckLhs(1, 1);
    CheckRhs(3, 3);

    err = getVarAddressFromPosition(pvApiCtx, 1, &indexaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isDoubleType(pvApiCtx, indexaddr))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: A double expected.\n"), fname, 1);
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
    a = XMLObject::getFromId<XMLNodeList>(lhsid);
    if (!a)
    {
        Scierror(999, gettext("%s: XML object does not exist.\n"), fname);
        return 0;
    }

    success = XMLRhsValue::get(fname, rhsaddr, &b, pvApiCtx);
    if (!success)
    {
        Scierror(999, gettext("%s: Error in getting rhs argument.\n"), fname);
        return 0;
    }

    a->setElementAtPosition(index, *b);

    if (typeid(T) == typeid(std::string))
    {
        delete b;
    }

    if (a->createOnStack(Rhs + 1, pvApiCtx))
    {
        LhsVar(1) = Rhs + 1;
    }
    else
    {
        LhsVar(1) = 0;
    }

    PutLhsVar();

    return 0;
}

#endif
