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
#include "XMLElement.hxx"
#include "XMLNs.hxx"
#include "XMLAttr.hxx"
#include "XMLNodeList.hxx"

extern "C"
{
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "sciprint.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_print(char *fname, void *pvApiCtx)
{
    XMLObject *obj;
    int id;
    SciErr err;
    int *mlistaddr = 0;

    CheckRhs(1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &mlistaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    id = getXMLObjectId(mlistaddr, pvApiCtx);
    obj = XMLObject::getFromId < XMLObject > (id);
    if (!obj)
    {
        Scierror(999, gettext("%s: XML object does not exist.\n"), fname);
        return 0;
    }

    const std::string str = obj->toString();

    sciprint("%s\n", str.c_str());

    return 0;
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLDoc_p(char *fname, void* pvApiCtx)
{
    return sci_print(fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLElem_p(char *fname, void* pvApiCtx)
{
    return sci_print(fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLNs_p(char *fname, void* pvApiCtx)
{
    return sci_print(fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLList_p(char *fname, void* pvApiCtx)
{
    return sci_print(fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLAttr_p(char *fname, void* pvApiCtx)
{
    return sci_print(fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLSet_p(char *fname, void* pvApiCtx)
{
    return sci_print(fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int sci_percent_XMLValid_p(char *fname, void* pvApiCtx)
{
    return sci_print(fname, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
