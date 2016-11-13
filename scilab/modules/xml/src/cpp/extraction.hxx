/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#include <vector>

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLElement.hxx"
#include "XMLNs.hxx"
#include "XMLAttr.hxx"
#include "XMLNodeList.hxx"

extern "C"
{
#include <string.h>
#include <stdio.h>
#include "gw_xml.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "xml_mlist.h"
#include "xml_constants.h"
#include "localization.h"
#include "os_string.h"
}

using namespace org_modules_xml;

/*--------------------------------------------------------------------------*/

/**
 * Creates a string on stack
 * @param fname the function name
 * @param str the string to put
 * @param pos the stack position
 * @return 1 if all is ok, else 0
 */
int createStringOnStack(char * fname, const char * str, int pos, void* pvApiCtx)
{
    SciErr err;

    if (!str)
    {
        str = "";
    }

    if (strchr(str, '\n'))
    {
        char * tok = os_strdup(str);
        char * stok = tok;
        std::vector<char *> vector = std::vector<char *>();

        tok = strtok(tok, "\n");
        while (tok)
        {
            vector.push_back(tok);
            tok = strtok(0, "\n");
        }

        if (vector.size())
        {
            err = createMatrixOfString(pvApiCtx, pos, (int)vector.size(), 1, const_cast<const char * const *>(&(vector[0])));
        }
        else
        {
            err = createMatrixOfDouble(pvApiCtx, pos, 0, 0, 0);
        }

        free(stok);
    }
    else
    {
        err = createMatrixOfString(pvApiCtx, pos, 1, 1, const_cast<const char * const *>(&str));
    }

    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    return 1;
}
/*--------------------------------------------------------------------------*/
/**
 * Creates a new variable on stack according to the requested field
 * @param fname the function name
 * @param doc the document
 * @param field the field name
 * @param pos the stack position
 * @return 1 if all is ok, else 0
 */
int createVariableOnStack(char * fname, org_modules_xml::XMLDocument & doc, const char * field, int pos, void* pvApiCtx)
{
    if (!strcmp("root", field))
    {
        const XMLElement * e = doc.getRoot();
        if (!e)
        {
            Scierror(999, gettext("%s: No root element.\n"), fname, field);
            return 0;
        }
        return e->createOnStack(pos, pvApiCtx);
    }
    else if (!strcmp("url", field))
    {
        return createStringOnStack(fname, doc.getDocumentURL(), pos, pvApiCtx);
    }
    else
    {
        Scierror(999, gettext("%s: Unknown field: %s\n"), fname, field);
        return 0;
    }
}
/*--------------------------------------------------------------------------*/

/**
 * Creates a new variable on stack according to the requested field
 * @param fname the function name
 * @param elem the element
 * @param field the field name
 * @param pos the stack position
 * @return 1 if all is ok, else 0
 */
int createVariableOnStack(char * fname, XMLElement & elem, const char * field, int pos, void* pvApiCtx)
{
    if (!strcmp("name", field))
    {
        return createStringOnStack(fname, elem.getNodeName(), pos, pvApiCtx);
    }
    else if (!strcmp("namespace", field))
    {
        const XMLNs * ns = elem.getNodeNameSpace();
        if (ns)
        {
            return ns->createOnStack(pos, pvApiCtx);
        }
        else
        {
            createMatrixOfDouble(pvApiCtx, pos, 0, 0, 0);
            return 1;
        }
    }
    else if (!strcmp("content", field))
    {
        const char * content = elem.getNodeContent();
        int ret = createStringOnStack(fname, content, pos, pvApiCtx);
        xmlFree(const_cast<char *>(content));
        return ret;
    }
    else if (!strcmp("type", field))
    {
        return createStringOnStack(fname, nodes_type[elem.getNodeType() - 1], pos, pvApiCtx);
    }
    else if (!strcmp("parent", field))
    {
        const XMLElement * parent = elem.getParentElement();
        if (parent)
        {
            return parent->createOnStack(pos, pvApiCtx);
        }
        else
        {
            createMatrixOfDouble(pvApiCtx, pos, 0, 0, 0);
            return 1;
        }
    }
    else if (!strcmp("attributes", field))
    {
        return elem.getAttributes()->createOnStack(pos, pvApiCtx);
    }
    else if (!strcmp("children", field))
    {
        return elem.getChildren()->createOnStack(pos, pvApiCtx);
    }
    else if (!strcmp("line", field))
    {
        double line = (double)elem.getDefinitionLine();
        SciErr err = createMatrixOfDouble(pvApiCtx, pos, 1, 1, &line);
        if (err.iErr)
        {
            printError(&err, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        return 1;
    }
    else
    {
        Scierror(999, gettext("%s: Unknown field: %s\n"), fname, field);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/

/**
 * Creates a new variable on stack according to the requested field
 * @param fname the function name
 * @param ns the namespace
 * @param field the field name
 * @param pos the stack position
 * @return 1 if all is ok, else 0
 */
int createVariableOnStack(char * fname, XMLNs & ns, const char * field, int pos, void* pvApiCtx)
{
    if (!strcmp("href", field))
    {
        return createStringOnStack(fname, ns.getHref(), pos, pvApiCtx);
    }
    else if (!strcmp("prefix", field))
    {
        return createStringOnStack(fname, ns.getPrefix(), pos, pvApiCtx);
    }
    else
    {
        Scierror(999, gettext("%s: Unknown field: %s\n"), fname, field);
        return 0;
    }
}
/*--------------------------------------------------------------------------*/

/**
 * Function to handle extraction in different XMLObjects
 * @param fname the function name
 * @param fname_len the function name length
 */
template<class T>
int sci_extraction(char * fname, void* pvApiCtx)
{
    T * t;
    int id;
    SciErr err;
    int * fieldaddr = 0;
    int * mlistaddr = 0;
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
        Scierror(999, gettext("%s: Wrong type for input argument #%i: string expected.\n"), fname, 1);
        return 0;
    }

    err = getVarAddressFromPosition(pvApiCtx, 2, &mlistaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, fieldaddr, &field) != 0)
    {
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }
    id = getXMLObjectId(mlistaddr, pvApiCtx);

    t = XMLObject::getFromId<T>(id);
    if (!t)
    {
        freeAllocatedSingleString(field);
        Scierror(999, gettext("%s: XML object does not exist.\n"), fname);
        return 0;
    }

    ret = createVariableOnStack(fname, *t, const_cast<char *>(field), Rhs + 1, pvApiCtx);
    freeAllocatedSingleString(field);
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
