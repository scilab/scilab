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
#include "XMLValidation.hxx"
#include "XMLValidationDTD.hxx"

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlValidate(char * fname, unsigned long fname_len)
{
    XMLValidation * validation = 0;
    org_modules_xml::XMLDocument * doc = 0;
    SciErr err;
    int * addr = 0;
    std::string error;
    int id;
    bool isValid;
    char ** path = 0;
    int row = 0;
    int col = 0;

    CheckLhs(1, 1);
    CheckRhs(1, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (isStringType(pvApiCtx, addr))
    {
        getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &path);
    }
    else if (isXMLDoc(addr))
    {
        id = getXMLObjectId(addr);
        doc = XMLObject::getFromId<org_modules_xml::XMLDocument>(id);
        if (!doc)
        {
            Scierror(999, gettext("%s: XML document does not exist\n"), fname);
            return 0;
        }
    }
    else
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%i: A matrix of strings or a XMLDoc expected.\n"), fname, 1);
        return 0;
    }

    if (Rhs == 2)
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            return 0;
        }

        if (!isXMLValid(addr))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%i: A %s expected.\n"), fname, 1, "XMLValid");
            return 0;
        }

        id = getXMLObjectId(addr);
        validation = XMLObject::getFromId<XMLValidation>(id);
        if (!validation)
        {
            Scierror(999, gettext("%s: XML validation file does not exist.\n"), fname);
            return 0;
        }
    }
    else
    {
	validation = new XMLValidationDTD(&error);
    }

    if (path)
    {
        std::string msg = std::string("");
        for (int i = 0; i < row * col; i++)
        {
            isValid = validation->validate(path[i], &error);
            if (!isValid)
            {
                char * s = new char[strlen(gettext("The file %s is not valid:\n%s\n")) + strlen(path[i]) + error.size() + 1];
                sprintf(s, gettext("The file %s is not valid:\n%s\n"), path[i], error.c_str());
                msg.append(s);
                delete [] s;
            }
        }
        if (!msg.empty())
        {
            Scierror(999, "%s: %s", fname, msg.c_str());
            return 0;
        }
    }
    else
    {
        isValid = validation->validate(*doc, &error);
        if (!isValid)
        {
            Scierror(999, gettext("%s: The file is not valid:\n%s"), fname, error.c_str());
            return 0;
        }
    }

    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
