/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "xml_mlist.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"

#define NB_XMLOBJECTS 8
static const char *XMLObjects[] = { "XMLDoc", "XMLElem", "XMLAttr", "XMLNs", "XMLList", "XMLNH", "XMLSet", "XMLValid" };

static const char *_XMLDoc[] = { "XMLDoc", "_id" };
static const char *_XMLElem[] = { "XMLElem", "_id" };
static const char *_XMLAttr[] = { "XMLAttr", "_id" };
static const char *_XMLNs[] = { "XMLNs", "_id" };
static const char *_XMLList[] = { "XMLList", "_id" };
static const char *_XMLNotHandled[] = { "XMLNH", "_id" };
static const char *_XMLSet[] = { "XMLSet", "_id" };
static const char *_XMLValid[] = { "XMLValid", "_id" };

static int compareStrToMlistType(const char **str, int nb, int *mlist, void *pvApiCtx);

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
    return compareStrToMlistType(XMLObjects, 1, mlist, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int isXMLElem(int *mlist, void *pvApiCtx)
{
    return compareStrToMlistType(XMLObjects + 1, 1, mlist, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int isXMLAttr(int *mlist, void *pvApiCtx)
{
    return compareStrToMlistType(XMLObjects + 2, 1, mlist, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int isXMLNs(int *mlist, void *pvApiCtx)
{
    return compareStrToMlistType(XMLObjects + 3, 1, mlist, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int isXMLList(int *mlist, void *pvApiCtx)
{
    return compareStrToMlistType(XMLObjects + 4, 1, mlist, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int isXMLNotHandled(int *mlist, void *pvApiCtx)
{
    return compareStrToMlistType(XMLObjects + 5, 1, mlist, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int isXMLSet(int *mlist, void *pvApiCtx)
{
    return compareStrToMlistType(XMLObjects + 6, 1, mlist, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int isXMLValid(int *mlist, void *pvApiCtx)
{
    return compareStrToMlistType(XMLObjects + 7, 1, mlist, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int isXMLObject(int *mlist, void *pvApiCtx)
{
    return compareStrToMlistType(XMLObjects, NB_XMLOBJECTS, mlist, pvApiCtx);
}

/*--------------------------------------------------------------------------*/
int isXMLObjects(const char ** types, int nb, int *mlist, void *pvApiCtx)
{
    return compareStrToMlistType(types, nb, mlist, pvApiCtx);
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
/**
 * Compare a strings to the mlist type
 * @param str an array of string
 * @param nb the strings number
 * @param mlist the mlist address
 * @return 0 if one of the strings is not the mlist type
 */
static int compareStrToMlistType(const char **str, int nb, int *mlist, void *pvApiCtx)
{
    char **mlist_type = NULL;
    int i = 0, type;
    int rows, cols;
    int *lengths = NULL;
    int cmp = 0;

    SciErr err = getVarType(pvApiCtx, mlist, &type);

    if (err.iErr || type != sci_mlist)
    {
        return 0;
    }

    err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, NULL, NULL);
    if (err.iErr || rows != 1 || cols <= 0)
    {
        return 0;
    }

    lengths = (int *)MALLOC(sizeof(int) * rows * cols);
    err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, lengths, NULL);
    if (err.iErr)
    {
        return 0;
    }

    mlist_type = (char **)MALLOC(sizeof(char *) * rows * cols);
    for (; i < rows * cols; i++)
    {
        mlist_type[i] = (char *)MALLOC(sizeof(char) * (lengths[i] + 1));
    }

    err = getMatrixOfStringInList(pvApiCtx, mlist, 1, &rows, &cols, lengths, mlist_type);
    if (err.iErr)
    {
        return 0;
    }

    for (i = 0; i < nb && !cmp; i++)
    {
        cmp = !strcmp(mlist_type[0], str[i]);
    }

    freeAllocatedMatrixOfString(rows, cols, mlist_type);
    FREE(lengths);

    if (cmp)
    {
        // useful when called by isXMLObject
        cmp = i;
    }

    return cmp;
}

/*--------------------------------------------------------------------------*/
