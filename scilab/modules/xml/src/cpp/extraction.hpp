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
#include <stdio.h>
#include "gw_xml.h"
#include "stack-c.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "xml_constants.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/
int createStringOnStack(char * fname, const char * str, int pos)
{
    SciErr err;

    if (!str)
    {
        str = "";
    }
    err = createMatrixOfString(pvApiCtx, pos, 1, 1, const_cast<const char * const *>(&str));
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    return 1;
}
/*--------------------------------------------------------------------------*/
int createVariableOnStack(char * fname, org_modules_xml::XMLDocument & doc, const char * field, int pos)
{
    if (!strcmp("root", field))
    {
	return doc.getRoot()->createOnStack(pos);
    }
    else if (!strcmp("url", field))
    {
        return createStringOnStack(fname, doc.getDocumentURL(), pos);
    }
    else
    {
        Scierror(999, "%s: Unknown field: %s\n", fname, field);
        return 0;
    }

    return 1;
}
/*--------------------------------------------------------------------------*/
int createVariableOnStack(char * fname, XMLElement & elem, const char * field, int pos)
{
    if (!strcmp("name", field))
    {
        return createStringOnStack(fname, elem.getNodeName(), pos);
    }
    else if (!strcmp("namespace", field))
    {
        return elem.getNodeNameSpace()->createOnStack(pos);
    }
    else if (!strcmp("content", field))
    {
        const char * content = elem.getNodeContent();
        int ierr = createStringOnStack(fname, content, pos);
        xmlFree(const_cast<char *>(content));
        return ierr;
    }
    else if (!strcmp("type", field))
    {
        return createStringOnStack(fname, nodes_type[elem.getNodeType() - 1], pos);
    }
    else if (!strcmp("parent", field))
    {
        return elem.getParentElement()->createOnStack(pos);
    }
    else if (!strcmp("attributes", field))
    {
        return elem.getAttributes()->createOnStack(pos);
    }
    else if (!strcmp("children", field))
    {
        return elem.getChildren()->createOnStack(pos);
    }
    else
    {
        Scierror(999, "%s: Unknown field: %s\n", fname, field);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int createVariableOnStack(char * fname, XMLNs & ns, const char * field, int pos)
{
    if (!strcmp("uri", field))
    {
        createStringOnStack(fname, ns.getURI(), pos);
    }
    else if (!strcmp("prefix", field))
    {
        createStringOnStack(fname, ns.getPrefix(), pos);
    }
    else
    {
        Scierror(999, "%s: Unknown field: %s\n", fname, field);
        return 0;
    }

    return 1;
}
/*--------------------------------------------------------------------------*/
int createVariableOnStack(char * fname, XMLAttr & attr, const char * field, int pos)
{
    const char * value = attr.getAttributeValue(field);
    int ret = createStringOnStack(fname, value, pos);
    if (value)
    {
        xmlFree(const_cast<char *>(value));
    }

    return ret;
}
/*--------------------------------------------------------------------------*/
template<class T>
int sci_extraction(char * fname, unsigned long fname_len)
{
    T * t;
    int id;
    SciErr err;
    int * fieldaddr = 0;
    int *mlistaddr = 0;
    char * field = 0;
    int ret;

    CheckLhs(1, 1);
    CheckRhs(2, 2);

    err = getVarAddressFromPosition(pvApiCtx, 1, &fieldaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (!isStringType(pvApiCtx, fieldaddr))
    {
        Scierror(999, "%s: Wrong type for input argument %i: String expected\n", fname, 1);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &mlistaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    getAllocatedSingleString(pvApiCtx, fieldaddr, &field);
    id = getXMLObjectId(mlistaddr);

    t = XMLObject::getFromId<T>(id);

    if (!t)
    {
        Scierror(999, "%s: XML object does not exist\n", fname);
        return 0;
    }

    ret = createVariableOnStack(fname, *t, const_cast<char *>(field), Rhs + 1);
    if (ret)
    {
        LhsVar(1) = Rhs + 1;
    }
    else
    {
        LhsVar(1) = 0;
    }
    PutLhsVar();

    return 0;
}
