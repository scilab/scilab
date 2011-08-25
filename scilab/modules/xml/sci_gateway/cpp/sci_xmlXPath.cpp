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
}

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLXPath.hxx"
#include "XMLNodeSet.hxx"

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlXPath(char * fname, unsigned long fname_len)
{
    int id;
    SciErr err;
    org_modules_xml::XMLDocument * doc;
    XMLXPath * xpath;
    XMLNodeSet * set;
    int b;
    double d;
    const char * str;
    int *addr = 0;
    char * query = 0;
    char *error = 0;

    CheckLhs(1, 1);
    CheckRhs(2, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isXMLDoc(addr))
    {
        Scierror(999, "%s: Wrong type for input argument %i: %s expected\n", fname, 1, "XMLDoc");
        return 0;
    }

    id = getXMLObjectId(addr);
    doc = XMLObject::getFromId<org_modules_xml::XMLDocument>(id);

    if (!doc)
    {
        Scierror(999, "%s: XML document does not exist\n", fname);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr))
    {
        Scierror(999, "%s: Wrong type for input argument %i: String expected\n", fname, 2);
        return 0;
    }

    getAllocatedSingleString(pvApiCtx, addr, &query);

    xpath = doc->makeXPathQuery(const_cast<const char *>(query), &error);

    if (error)
    {
	Scierror(999, "%s: Bad XPath query:\n%s", fname, error);
        return 0;
    }

    switch (xpath->getResultType())
    {
    case XPATH_NODESET :;
	set = xpath->getNodeSet();
	id = set->getId();
        if (!createXMLObjectAtPos(XMLLIST, Rhs + 1, id))
        {
            return 0;
        }
	break;
    case XPATH_BOOLEAN :;
	b = xpath->getBooleanValue();
	createScalarBoolean(pvApiCtx, Rhs + 1, b);
	break;
    case XPATH_NUMBER :;
	d = xpath->getFloatValue();
	createScalarDouble(pvApiCtx, Rhs + 1, d);
	break;
    case XPATH_STRING :;
	str = xpath->getStringValue();
	createSingleString(pvApiCtx, Rhs + 1, str);
	break;
    default :;
	Scierror(999, "%s: XPath query returned a not handled type: %i\n", fname, xpath->getResultType());
        return 0;
    }
    
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
