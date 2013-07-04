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
#include "XMLElement.hxx"
#include "XMLAttr.hxx"

extern "C"
{
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

#include <iostream>

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlName(char *fname, unsigned long fname_len)
{
    int id;
    SciErr err;
    int *addr = 0;
    const char **pstStrings = 0;
    const char * types[] = {"XMLAttr", "XMLList", "XMLSet", "XMLElem"};
    int type;
    int size;

    CheckLhs(1, 1);
    CheckRhs(1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    type = isXMLObjects(types, 4, addr, pvApiCtx);
    if (!type)
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: XMLSet, XMLList, XMLAttr or XMLElem expected.\n"), fname, 1);
        return 0;
    }
    type--;

    id = getXMLObjectId(addr, pvApiCtx);

    if (type == 1 || type == 2)
    {
        XMLList * list = XMLObject::getFromId < XMLList > (id);
        if (!list)
        {
            Scierror(999, gettext("%s: XMLSet or XMLList does not exist.\n"), fname);
            return 0;
        }

        pstStrings = list->getNameFromList();
        size = list->getSize();
    }
    else if (type == 0)
    {
        XMLAttr * attrs = XMLObject::getFromId<XMLAttr>(id);
        if (!attrs)
        {
            Scierror(999, gettext("%s: XMLAttr does not exist.\n"), fname);
            return 0;
        }

        pstStrings = attrs->getNames();
        size = attrs->getSize();
    }
    else
    {
        XMLElement * elem = XMLObject::getFromId<XMLElement>(id);
        if (!elem)
        {
            Scierror(999, gettext("%s: XMLElem does not exist.\n"), fname);
            return 0;
        }

        pstStrings = new const char *[1];
        pstStrings[0] = elem->getNodeName();
        size = 1;
    }

    if (size)
    {
        err = createMatrixOfString(pvApiCtx, Rhs + 1, 1, size, const_cast < const char * const *>(pstStrings));
        delete[]pstStrings;
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
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
