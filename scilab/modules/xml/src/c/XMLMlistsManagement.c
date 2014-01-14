/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012-2014 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "xml_mlist.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"

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
int createXMLObjectAtPos(int type, int pos, int id, void *pvApiCtx)
{
    const char **fields = NULL;
    int *mlistaddr = NULL;
    SciErr err;

    switch (type)
    {
        case XMLDOCUMENT:
            ;
            fields = _XMLDoc;
            break;
        case XMLELEMENT:
            ;
            fields = _XMLElem;
            break;
        case XMLATTRIBUTE:
            ;
            fields = _XMLAttr;
            break;
        case XMLNAMESPACE:
            ;
            fields = _XMLNs;
            break;
        case XMLLIST:
            ;
            fields = _XMLList;
            break;
        case XMLNOTHANDLED:
            ;
            fields = _XMLNotHandled;
            break;
        case XMLSET:
            ;
            fields = _XMLSet;
            break;
        case XMLVALID:
            ;
            fields = _XMLValid;
            break;
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
            ;
            fields = _XMLDoc;
            break;
        case XMLELEMENT:
            ;
            fields = _XMLElem;
            break;
        case XMLATTRIBUTE:
            ;
            fields = _XMLAttr;
            break;
        case XMLNAMESPACE:
            ;
            fields = _XMLNs;
            break;
        case XMLLIST:
            ;
            fields = _XMLList;
            break;
        case XMLNOTHANDLED:
            ;
            fields = _XMLNotHandled;
            break;
        case XMLSET:
            ;
            fields = _XMLSet;
            break;
        case XMLVALID:
            ;
            fields = _XMLValid;
            break;
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
    if (mlist[0] != sci_mlist || mlist[1] != 2)
    {
        return -1;
    }

    if (mlist[6] != sci_strings || mlist[7] != 1 || mlist[8] != 2)
    {
        // first field is not a matrix 1x2 of strings
        return -1;
    }

    if (mlist[13] == -33 && mlist[14] == -22 && mlist[15] == -21)
    {
        if (mlist[11] - 1 == strlen("XMLDoc") && mlist[16] == -13 && mlist[17] == 24  && mlist[18] == 12)
        {
            return XMLDOCUMENT;
        }
        if (mlist[11] - 1 == strlen("XMLElem") && mlist[16] == -14 && mlist[17] == 21 && mlist[18] == 14 && mlist[19] == 22)
        {
            return XMLELEMENT;
        }
        if (mlist[11] - 1 == strlen("XMLAttr") && mlist[16] == -10 && mlist[17] == 29 && mlist[18] == 29 && mlist[19] == 27)
        {
            return XMLATTRIBUTE;
        }
        if (mlist[11] - 1 == strlen("XMLNs") && mlist[16] == -23 && mlist[17] == 28)
        {
            return XMLNAMESPACE;
        }
        if (mlist[11] - 1 == strlen("XMLList") && mlist[16] == -21 && mlist[17] == 18 && mlist[18] == 28 && mlist[19] == 29)
        {
            return XMLLIST;
        }
        if (mlist[11] - 1 == strlen("XMLNH") && mlist[16] == -23 && mlist[17] == 17)
        {
            return XMLNOTHANDLED;
        }
        if (mlist[11] - 1 == strlen("XMLSet") && mlist[16] == -28 && mlist[17] == 14 && mlist[18] == 29)
        {
            return XMLSET;
        }
        if (mlist[11] - 1 == strlen("XMLValid") && mlist[16] == -31 && mlist[17] == 10 && mlist[18] == 21 && mlist[19] == 18 && mlist[20] == 13)
        {
            return XMLVALID;
        }
    }

    return -1;
}
