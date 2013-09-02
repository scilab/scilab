/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLValidation.hxx"

extern "C"
{
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlGetOpenDocs(char *fname, unsigned long fname_len)
{
    int j = 1;
    SciErr err;
    int *addr = 0;

    CheckLhs(1, 1);
    CheckRhs(0, 0);

    const std::list < org_modules_xml::XMLDocument * >&openDocs = org_modules_xml::XMLDocument::getOpenDocuments();
    const std::list < org_modules_xml::XMLValidation * >&openValidationFiles = org_modules_xml::XMLValidation::getOpenValidationFiles();

    err = createList(pvApiCtx, Rhs + 1, (int)openDocs.size() + (int)openValidationFiles.size(), &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    for (std::list < org_modules_xml::XMLDocument * >::const_iterator i = openDocs.begin(); i != openDocs.end(); i++, j++)
    {
        createXMLObjectAtPosInList(addr, Rhs + 1, XMLDOCUMENT, j, (*i)->getId(), pvApiCtx);
    }

    for (std::list < org_modules_xml::XMLValidation * >::const_iterator i = openValidationFiles.begin(); i != openValidationFiles.end(); i++, j++)
    {
        createXMLObjectAtPosInList(addr, Rhs + 1, XMLVALID, j, (*i)->getId(), pvApiCtx);
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
