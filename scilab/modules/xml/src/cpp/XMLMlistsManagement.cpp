/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012-2014 - Scilab Enterprises - Calixte DENIZET
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

#include "mlist.hxx"
#include "tlist.hxx"
#include "internal.hxx"
#include "string.hxx"

extern "C"
{
#include <string.h>
#include "xml_mlist.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
}

static const char *_XMLDoc[] = { "XMLDoc", "_id" };
static const char *_XMLElem[] = { "XMLElem", "_id" };
static const char *_XMLAttr[] = { "XMLAttr", "_id" };
static const char *_XMLNs[] = { "XMLNs", "_id" };
static const char *_XMLList[] = { "XMLList", "_id" };
static const char *_XMLNotHandled[] = { "XMLNH", "_id" };
static const char *_XMLSet[] = { "XMLSet", "_id" };
static const char *_XMLValid[] = { "XMLValid", "_id" };

static int getMListType(int * mlist, void * pvApiCtx);

/*--------------------------------------------------------------------------*/
int createXMLObjectAtPos(int type, int pos, int id, void* pvApiCtx)
{
    const char **fields = NULL;
    int *mlistaddr = NULL;
    SciErr err;

    switch (type)
    {
        case XMLDOCUMENT:
            fields = _XMLDoc;
            break;
        case XMLELEMENT:
            fields = _XMLElem;
            break;
        case XMLATTRIBUTE:
            fields = _XMLAttr;
            break;
        case XMLNAMESPACE:
            fields = _XMLNs;
            break;
        case XMLLIST:
            fields = _XMLList;
            break;
        case XMLNOTHANDLED:
            fields = _XMLNotHandled;
            break;
        case XMLSET:
            fields = _XMLSet;
            break;
        case XMLVALID:
            fields = _XMLValid;
            break;
        default:
            Scierror(999, _("Unknown tag.\n"));
            return 0;
    }

    err = createMList(pvApiCtx, pos, 2, &mlistaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    err = createMatrixOfStringInList(pvApiCtx, pos, mlistaddr, 1, 1, 2, fields);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    err = createMatrixOfInteger32InList(pvApiCtx, pos, mlistaddr, 2, 1, 1, &id);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }
    return 1;
}

/*--------------------------------------------------------------------------*/
int createXMLObjectAtPosInList(int *list, int stackPos, int type, int pos, int id, void *pvApiCtx)
{
    const char **fields = NULL;
    int *mlistaddr = NULL;
    SciErr err;

    err = createMListInList(pvApiCtx, stackPos, list, pos, 2, &mlistaddr);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    switch (type)
    {
        case XMLDOCUMENT:
            fields = _XMLDoc;
            break;
        case XMLELEMENT:
            fields = _XMLElem;
            break;
        case XMLATTRIBUTE:
            fields = _XMLAttr;
            break;
        case XMLNAMESPACE:
            fields = _XMLNs;
            break;
        case XMLLIST:
            fields = _XMLList;
            break;
        case XMLNOTHANDLED:
            fields = _XMLNotHandled;
            break;
        case XMLSET:
            fields = _XMLSet;
            break;
        case XMLVALID:
            fields = _XMLValid;
            break;
        default:
            Scierror(999, _("Unknown tag.\n"));
            return 0;
    }

    err = createMatrixOfStringInList(pvApiCtx, stackPos, mlistaddr, 1, 1, 2, fields);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    err = createMatrixOfInteger32InList(pvApiCtx, stackPos, mlistaddr, 2, 1, 1, &id);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    return 1;
}

/*--------------------------------------------------------------------------*/
int isXMLDoc(int *mlist, void *pvApiCtx)
{
    return getMListType(mlist, pvApiCtx) == XMLDOCUMENT;
}

/*--------------------------------------------------------------------------*/
int isXMLElem(int *mlist, void *pvApiCtx)
{
    return getMListType(mlist, pvApiCtx) == XMLELEMENT;
}

/*--------------------------------------------------------------------------*/
int isXMLAttr(int *mlist, void *pvApiCtx)
{
    return getMListType(mlist, pvApiCtx) == XMLATTRIBUTE;
}

/*--------------------------------------------------------------------------*/
int isXMLNs(int *mlist, void *pvApiCtx)
{
    return getMListType(mlist, pvApiCtx) == XMLNAMESPACE;
}

/*--------------------------------------------------------------------------*/
int isXMLList(int *mlist, void *pvApiCtx)
{
    return getMListType(mlist, pvApiCtx) == XMLLIST;
}

/*--------------------------------------------------------------------------*/
int isXMLNotHandled(int *mlist, void *pvApiCtx)
{
    return getMListType(mlist, pvApiCtx) == XMLNOTHANDLED;
}

/*--------------------------------------------------------------------------*/
int isXMLSet(int *mlist, void *pvApiCtx)
{
    return getMListType(mlist, pvApiCtx) == XMLSET;
}

/*--------------------------------------------------------------------------*/
int isXMLValid(int *mlist, void *pvApiCtx)
{
    return getMListType(mlist, pvApiCtx) == XMLVALID;
}

/*--------------------------------------------------------------------------*/
int isXMLObject(int *mlist, void *pvApiCtx)
{
    const int type = getMListType(mlist, pvApiCtx);
    return type == XMLDOCUMENT || type == XMLELEMENT || type == XMLATTRIBUTE || type == XMLNAMESPACE || type == XMLLIST || type == XMLNOTHANDLED || type == XMLSET || type == XMLVALID;
}

/*--------------------------------------------------------------------------*/
int isXMLObjects(const int * types, int nb, int *mlist, void *pvApiCtx)
{
    const int type = getMListType(mlist, pvApiCtx);
    int i = 0;
    for (; i < nb; i++)
    {
        if (type == types[i])
        {
            return i;
        }
    }

    return -1;
}
/*--------------------------------------------------------------------------*/
int getXMLObjectId(int *mlist, void *pvApiCtx)
{
    int *id = NULL;
    int row, col;

    SciErr err = getMatrixOfInteger32InList(pvApiCtx, mlist, 2, &row, &col, &id);

    if (err.iErr)
    {
        printError(&err, 0);
        return -1;
    }

    return *id;
}

/*--------------------------------------------------------------------------*/
int getMListType(int * mlist, void * pvApiCtx)
{
    types::InternalType* pIT = (types::InternalType*)mlist;
    types::MList* m = dynamic_cast<types::MList*>(pIT);

    if (m == NULL || m->getSize() != 2)
    {
        return -1;
    }

    types::String* pS = m->getFieldNames();

    if (pS->getRows() != 1 || pS->getCols() != 2)
    {
        // first field is not a matrix 1x2 of strings
        return -1;
    }

    wchar_t* pwstType = pS->get(0);
    int iLen = (int)wcslen(pwstType);

    if (wcsncmp(pwstType, L"XML", 3) == 0)
    {
        if (iLen == 6 /*strlen("XMLDoc")*/ && wcscmp(pwstType + 3, L"Doc") == 0)
        {
            return XMLDOCUMENT;
        }
        if (iLen == 7 /*strlen("XMLElem")*/ && wcscmp(pwstType + 3, L"Elem") == 0)
        {
            return XMLELEMENT;
        }
        if (iLen == 7 /*strlen("XMLAttr")*/ && wcscmp(pwstType + 3, L"Attr") == 0)
        {
            return XMLATTRIBUTE;
        }
        if (iLen == 5 /*strlen("XMLNs")*/ && wcscmp(pwstType + 3, L"Ns") == 0)
        {
            return XMLNAMESPACE;
        }
        if (iLen == 7 /*strlen("XMLList")*/ && wcscmp(pwstType + 3, L"List") == 0)
        {
            return XMLLIST;
        }
        if (iLen == 5 /*strlen("XMLNH")*/ && wcscmp(pwstType + 3, L"NH") == 0)
        {
            return XMLNOTHANDLED;
        }
        if (iLen == 6 /*strlen("XMLSet")*/ && wcscmp(pwstType + 3, L"Set") == 0)
        {
            return XMLSET;
        }
        if (iLen == 8 /*strlen("XMLValid")*/ && wcscmp(pwstType + 3, L"Valid") == 0)
        {
            return XMLVALID;
        }
    }

    return -1;
}
