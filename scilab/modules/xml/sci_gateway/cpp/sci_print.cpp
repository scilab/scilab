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
#include "XMLElement.hxx"
#include "XMLNs.hxx"
#include "XMLAttr.hxx"
#include "XMLNodeList.hxx"

extern "C"
{
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "sciprint.h"
#include "xml_mlist.h"
#include "localization.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int sci_print(char * fname, int* _piKey)
{
    XMLObject * obj;
    int id;
    SciErr err;
    int * mlistaddr = 0;

    CheckRhs(1, 1);

    err = getVarAddressFromPosition(_piKey, 1, &mlistaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    id = getXMLObjectId(mlistaddr, _piKey);
    obj = XMLObject::getFromId<XMLObject>(id);
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
int sci_percent_XMLDoc_p(char *fname, int* _piKey)
{
    return sci_print(fname, _piKey);
}
/*--------------------------------------------------------------------------*/
int sci_percent_XMLElem_p(char *fname, int* _piKey)
{
    return sci_print(fname, _piKey);
}
/*--------------------------------------------------------------------------*/
int sci_percent_XMLNs_p(char *fname, int* _piKey)
{
    return sci_print(fname, _piKey);
}
/*--------------------------------------------------------------------------*/
int sci_percent_XMLList_p(char *fname, int* _piKey)
{
    return sci_print(fname, _piKey);
}
/*--------------------------------------------------------------------------*/
int sci_percent_XMLAttr_p(char *fname, int* _piKey)
{
    return sci_print(fname, _piKey);
}
/*--------------------------------------------------------------------------*/
int sci_percent_XMLSet_p(char *fname, int* _piKey)
{
    return sci_print(fname, _piKey);
}
/*--------------------------------------------------------------------------*/
int sci_percent_XMLValid_p(char *fname, int* _piKey)
{
    return sci_print(fname, _piKey);
}
/*--------------------------------------------------------------------------*/
