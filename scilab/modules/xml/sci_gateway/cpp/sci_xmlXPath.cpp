/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


extern "C"
{
#include "xml.h"
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLXPath.hxx"
#include "XMLNodeSet.hxx"

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlXPath(char * fname, int *_piKey)
{
    int id;
    SciErr err;
    org_modules_xml::XMLDocument * doc;
    const XMLXPath * xpath;
    int * addr = 0;
    char * query = 0;
    std::string error;
    int row = 0;
    int col = 0;
    char ** namespaces = 0;

    CheckLhs(1, 1);
    CheckRhs(2, 3);

    err = getVarAddressFromPosition(_piKey, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isXMLDoc(addr, _piKey))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: A %s expected.\n"), fname, 1, "XMLDoc");
        return 0;
    }

    id = getXMLObjectId(addr, _piKey);
    doc = XMLObject::getFromId<org_modules_xml::XMLDocument>(id);
    if (!doc)
    {
        Scierror(999, gettext("%s: XML document does not exist.\n"), fname);
        return 0;
    }

    err = getVarAddressFromPosition(_piKey, 2, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isStringType(_piKey, addr))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: A string expected.\n"), fname, 2);
        return 0;
    }
    getAllocatedSingleString(_piKey, addr, &query);

    if (Rhs == 3)
    {
        err = getVarAddressFromPosition(_piKey, 3, &addr);
        if (err.iErr)
        {
            freeAllocatedSingleString(query);
            printError(&err, 0);
            return 0;
        }

        if (!isStringType(_piKey, addr))
        {
            freeAllocatedSingleString(query);
            Scierror(999, gettext("%s: Wrong type for input argument #%i: A string expected.\n"), fname, 2);
            return 0;
        }

        err = getMatrixOfString(_piKey, addr, &row, &col, 0, 0);
        if (err.iErr)
        {
            freeAllocatedSingleString(query);
            printError(&err, 0);
            return 0;
        }

        if (col != 2)
        {
            freeAllocatedSingleString(query);
            Scierror(999, gettext("%s: Bad number of columns for argument #%i: two expected.\n"), fname, 3);
            return 0;
        }

        getAllocatedMatrixOfString(_piKey, addr, &row, &col, &namespaces);
    }

    xpath = doc->makeXPathQuery(const_cast<const char *>(query), namespaces, row, &error);
    freeAllocatedSingleString(query);
    if (namespaces)
    {
        freeAllocatedMatrixOfString(row, col, namespaces);
    }

    if (!error.empty())
    {
        Scierror(999, gettext("%s: Bad XPath query:\n%s"), fname, error.c_str());
        return 0;
    }

    switch (xpath->getResultType())
    {
    case XPATH_NODESET :
    {
        const XMLNodeSet * set = xpath->getNodeSet();
        if (set->getSize() == 0)
        {
            createMatrixOfDouble(_piKey, Rhs + 1, 0, 0, 0);
        }
        set->createOnStack(Rhs + 1, _piKey);
        break;
    }
    case XPATH_BOOLEAN :
    {
        int b = xpath->getBooleanValue();
        createScalarBoolean(_piKey, Rhs + 1, b);
        break;
    }
    case XPATH_NUMBER :
    {
        double d = xpath->getFloatValue();
        createScalarDouble(_piKey, Rhs + 1, d);
        break;
    }
    case XPATH_STRING :
    {
        const char * str = xpath->getStringValue();
        createSingleString(_piKey, Rhs + 1, str);
        break;
    }
    default :
        delete xpath;
        Scierror(999, gettext("%s: XPath query returned a not handled type: %i\n"), fname, xpath->getResultType());
        return 0;
    }

    delete xpath;

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
