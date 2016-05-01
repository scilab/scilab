/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte Denizet
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

#include "xml.h"

extern "C"
{
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLElement.hxx"
#include "XMLXPath.hxx"
#include "XMLNodeSet.hxx"

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlXPath(char* fname, void* pvApiCtx)
{
    int id;
    SciErr err;

    org_modules_xml::XMLDocument * doc;
    XMLElement *where = 0;
    const XMLXPath *xpath;
    int *addr = 0;
    char *query = 0;

    std::string error;
    int row = 0;
    int col = 0;
    char **namespaces = 0;
    int isElem = 0;
    bool mustDelete = true;

    CheckLhs(1, 1);
    CheckRhs(2, 3);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    isElem = isXMLElem(addr, pvApiCtx);

    if (!isElem && !isXMLDoc(addr, pvApiCtx))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A XMLDoc or a XMLElem expected.\n"), fname, 1);
        return 0;
    }

    id = getXMLObjectId(addr, pvApiCtx);
    if (isElem)
    {
        where = XMLObject::getFromId < XMLElement > (id);
        if (!where)
        {
            Scierror(999, gettext("%s: XML element does not exist.\n"), fname);
            return 0;
        }
        doc = const_cast < org_modules_xml::XMLDocument * >(&(where->getXMLDocument()));
    }
    else
    {
        doc = XMLObject::getFromId < org_modules_xml::XMLDocument > (id);
        if (!doc)
        {
            Scierror(999, gettext("%s: XML document does not exist.\n"), fname);
            return 0;
        }
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &query) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    if (Rhs == 3)
    {
        err = getVarAddressFromPosition(pvApiCtx, 3, &addr);
        if (err.iErr)
        {
            freeAllocatedSingleString(query);
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if (!isStringType(pvApiCtx, addr))
        {
            freeAllocatedSingleString(query);
            Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
            return 0;
        }

        err = getMatrixOfString(pvApiCtx, addr, &row, &col, 0, 0);
        if (err.iErr)
        {
            freeAllocatedSingleString(query);
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (col != 2)
        {
            freeAllocatedSingleString(query);
            Scierror(999, gettext("%s: Bad number of columns for argument #%d: two expected.\n"), fname, 3);
            return 0;
        }

        if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &namespaces) != 0)
        {
            freeAllocatedSingleString(query);
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }

    xpath = doc->makeXPathQuery(const_cast < const char *>(query), namespaces, row, where, &error);
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
        case XPATH_NODESET:
        {
            const XMLNodeSet *set = xpath->getNodeSet();

            if (set->getSize() == 0)
            {
                createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, 0);
            }
            set->createOnStack(Rhs + 1, pvApiCtx);
            mustDelete = false;
            break;
        }
        case XPATH_BOOLEAN:
        {
            int b = xpath->getBooleanValue();

            createScalarBoolean(pvApiCtx, Rhs + 1, b);
            break;
        }
        case XPATH_NUMBER:
        {
            double d = xpath->getFloatValue();

            createScalarDouble(pvApiCtx, Rhs + 1, d);
            break;
        }
        case XPATH_STRING:
        {
            const char *str = xpath->getStringValue();

            createSingleString(pvApiCtx, Rhs + 1, str);
            break;
        }
        default:
            Scierror(999, gettext("%s: XPath query returned a not handled type: %i\n"), fname, xpath->getResultType());
            delete xpath;
            return 0;
    }

    if (mustDelete)
    {
        xmlXPathObject *real = static_cast < xmlXPathObject * >(xpath->getRealXMLPointer());
        delete xpath;

        xmlXPathFreeObject(real);
    }
    else
    {
        delete xpath;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
