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
        Scierror(999, gettext("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
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
int sci_xmlDTD(char *fname, void* pvApiCtx)
{
    return sci_xmlValidationFile < XMLValidationDTD > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_xmlRelaxNG(char *fname, void* pvApiCtx)
{
    return sci_xmlValidationFile < XMLValidationRelaxNG > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_xmlSchema(char *fname, void* pvApiCtx)
{
    return sci_xmlValidationFile < XMLValidationSchema > (fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
