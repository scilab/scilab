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
#include "XMLValidation.hxx"

extern "C"
{
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlDelete(char * fname, int* _piKey)
{
    int id;
    SciErr err;
    int * addr = 0;
    org_modules_xml::XMLDocument * doc = 0;
    XMLValidation * vf = 0;
    char * com = 0;

    CheckLhs(1, 1);

    if (Rhs == 0)
    {
        Scierror(999, gettext("%s: Wrong number of input arguments: at least %d expected.\n"), fname, 1);
        return 0;
    }

    err = getVarAddressFromPosition(_piKey, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(_piKey, addr))
    {
        getAllocatedSingleString(_piKey, addr, &com);
        if (!strcmp(com, "all"))
        {
            org_modules_xml::XMLDocument::closeAllDocuments();
            XMLValidation::closeAllValidationFiles();
        }
        freeAllocatedSingleString(com);
    }
    else
    {
        for (int pos = 1; pos <= Rhs; pos++)
        {
            err = getVarAddressFromPosition(_piKey, pos, &addr);
            if (err.iErr)
            {
                printError(&err, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, pos);
                return 0;
            }

            if (isXMLDoc(addr, _piKey))
            {
                id = getXMLObjectId(addr, _piKey);
                doc = XMLObject::getFromId<org_modules_xml::XMLDocument>(id);
                if (!doc)
                {
                    Scierror(999, gettext("%s: XML document does not exist.\n"), fname);
                    return 0;
                }
                delete doc;
            }
            else if (isXMLValid(addr, _piKey))
            {
                id = getXMLObjectId(addr, _piKey);
                vf = XMLObject::getFromId<XMLValidation>(id);
                if (!vf)
                {
                    Scierror(999, gettext("%s: XML validation file does not exist.\n"), fname);
                    return 0;
                }
                delete vf;
            }
            else
            {
                Scierror(999, gettext("%s: Wrong type for input argument #%d: A %s expected\n"), fname, 1, "XMLDoc");
                return 0;
            }
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
