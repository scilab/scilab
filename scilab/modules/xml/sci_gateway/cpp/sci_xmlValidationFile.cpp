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
#include "XMLDocument.hxx"
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
#include "XMLValidation.hxx"
#include "XMLValidationDTD.hxx"
#include "XMLValidationSchema.hxx"
#include "XMLValidationRelaxNG.hxx"

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
template < class T > int sci_xmlValidationFile(char *fname, void *pvApiCtx)
{
    T *validation = 0;
    SciErr err;
    int *addr = 0;
    char *path = 0;

    std::string error;

    CheckLhs(1, 1);
    CheckRhs(1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &addr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isStringType(pvApiCtx, addr) || !checkVarDimension(pvApiCtx, addr, 1, 1))
    {
        Scierror(999, gettext("%s: Wrong type for input argument #%d: A string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, addr, &path) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    validation = new T((const char *)path, &error);
    freeAllocatedSingleString(path);

    if (!error.empty())
    {
        delete validation;

        Scierror(999, gettext("%s: Cannot read the file:\n%s"), fname, error.c_str());
        return 0;
    }

    if (!validation->createOnStack(Rhs + 1, pvApiCtx))
    {
        return 0;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}

/*--------------------------------------------------------------------------*/
int sci_xmlDTD(char *fname, unsigned long fname_len)
{
    return sci_xmlValidationFile < XMLValidationDTD > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_xmlRelaxNG(char *fname, unsigned long fname_len)
{
    return sci_xmlValidationFile < XMLValidationRelaxNG > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_xmlSchema(char *fname, unsigned long fname_len)
{
    return sci_xmlValidationFile < XMLValidationSchema > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
