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
#include "XMLValidation.hxx"
#include "XMLValidationDTD.hxx"
#include "SplitString.hxx"

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_xmlValidate(char *fname, void* pvApiCtx)
{
    XMLValidation *validation = 0;

    org_modules_xml::XMLDocument * doc = 0;
    SciErr err;
    int *addr = 0;

    std::string error;
    std::string msg;
    int id;
    bool isValid;
    char **path = 0;
    int row = 0;
    int col = 0;

    CheckLhs(1, 1);
    CheckRhs(1, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, addr))
    {
        if (getAllocatedMatrixOfString(pvApiCtx, addr, &row, &col, &path) != 0)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }
    }
    else if (isXMLDoc(addr, pvApiCtx))
    {
        id = getXMLObjectId(addr, pvApiCtx);
        doc = XMLObject::getFromId < org_modules_xml::XMLDocument > (id);
        if (!doc)
        {
            Scierror(999, gettext("%s: XML document does not exist.\n"), fname);
            return 0;
        }
    }
    else
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A matrix of strings or a XMLDoc expected.\n"), fname, 1);
        return 0;
    }

    if (Rhs == 2)
    {
        err = getVarAddressFromPosition(pvApiCtx, 2, &addr);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            if (path)
            {
                freeAllocatedMatrixOfString(row, col, path);
            }
            return 0;
        }

        if (!isXMLValid(addr, pvApiCtx))
        {
            Scierror(999, gettext("%s: Wrong type for input argument #%d: A %s expected.\n"), fname, 1, "XMLValid");
            if (path)
            {
                freeAllocatedMatrixOfString(row, col, path);
            }
            return 0;
        }

        id = getXMLObjectId(addr, pvApiCtx);
        validation = XMLObject::getFromId < XMLValidation > (id);
        if (!validation)
        {
            Scierror(999, gettext("%s: XML validation file does not exist.\n"), fname);
            if (path)
            {
                freeAllocatedMatrixOfString(row, col, path);
            }
            return 0;
        }
    }
    else
    {
        validation = new XMLValidationDTD();
    }

    if (path)
    {
        msg = std::string("");
        for (int i = 0; i < row * col; i++)
        {
            isValid = validation->validate(path[i], &error);
            if (!isValid)
            {
                char *s = new char[strlen(gettext("The file %s is not valid:\n%s\n")) + strlen(path[i]) + error.size() + 1];

                sprintf(s, gettext("The file %s is not valid:\n%s\n"), path[i], error.c_str());
                msg.append(s);
                delete[]s;
            }
        }
        freeAllocatedMatrixOfString(row, col, path);
    }
    else
    {
        isValid = validation->validate(*doc, &error);
        if (!isValid)
        {
            msg = error;
        }
    }

    if (!msg.empty())
    {
        std::vector < std::string > lines = std::vector < std::string > ();
        SplitString::split(msg, lines);
        std::vector < const char *>clines = std::vector < const char *>(lines.size());

        for (unsigned int i = 0; i < lines.size(); i++)
        {
            clines[i] = lines[i].c_str();
        }

        if (clines.size())
        {
            err = createMatrixOfString(pvApiCtx, Rhs + 1, (int)lines.size(), 1, const_cast < const char * const *>(&(clines[0])));
        }
        else
        {
            err = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, 0);
        }
    }
    else
    {
        err = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, 0);
    }

    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}

/*--------------------------------------------------------------------------*/
