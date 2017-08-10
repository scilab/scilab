/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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

/*------------------------------------------------------------------------*/
/* file: getPropertyAssignedValue.c                                       */
/* desc : a set of functions used to get the values which will be         */
/*        assigned to handles properties from the stack                   */
/*------------------------------------------------------------------------*/

#include "getPropertyAssignedValue.h"
#include "api_scilab.h"
#include "localization.h"
#include "sci_malloc.h"
#include "BasicAlgos.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
void copyDoubleVectorToIntFromStack(void* _pvData, int* _piDest, int _iNbItem)
{
    int i = 0;
    double* values = (double*)_pvData;
    for (i = 0 ; i < _iNbItem ; i++)
    {
        _piDest[i] = (int) values[i];
    }
}
/*--------------------------------------------------------------------------*/
char ** createCopyStringMatrixFromStack(void* _pvData, int _iNbItem)
{
    int i = 0;
    char ** res    = (char**)MALLOC(_iNbItem * sizeof(char *));
    char ** values = (char**)_pvData;

    if (res == NULL)
    {
        return NULL;
    }

    for (i = 0 ; i < _iNbItem ; i++)
    {
        res[i] = os_strdup(values[i]);
    }

    return res;

}
/*--------------------------------------------------------------------------*/
int tryGetBooleanValueFromStack(void* _pvData, int _iType, int _iRows, int _iCols, char const* _pstPropertyName)
{
    if (_iType == sci_strings)
    {
        if (stricmp((char*)_pvData, "on") == 0)
        {
            return TRUE;
        }
        if (stricmp((char*)_pvData, "off") == 0)
        {
            return FALSE;
        }
        if (stricmp((char*)_pvData, "1") == 0)
        {
            return TRUE;
        }
        if (stricmp((char*)_pvData, "0") == 0)
        {
            return FALSE;
        }
        if (stricmp((char*)_pvData, "T") == 0)
        {
            return TRUE;
        }
        if (stricmp((char*)_pvData, "F") == 0)
        {
            return FALSE;
        }

        Scierror(999, _("Wrong value for '%s' property: '%s' or '%s' expected.\n"), _pstPropertyName, "on", "off");
        return NOT_A_BOOLEAN_VALUE;
    }

    if (_iType == sci_boolean)
    {
        return ((int*)_pvData)[0];
    }

    if (_iType == sci_matrix)
    {
        if (((double*)_pvData)[0] == 0)
        {
            return FALSE;
        }
        return TRUE;
    }

    Scierror(999, _("Wrong type for '%s' property: String expected.\n"), _pstPropertyName);
    return NOT_A_BOOLEAN_VALUE;
}
/*--------------------------------------------------------------------------*/
int getStackListNbElement(void* _pvCtx, int _iRhs)
{
    int* piAddr = 0;
    int iItem = 0;

    getVarAddressFromPosition(_pvCtx, _iRhs, &piAddr);
    getListItemNumber(_pvCtx, piAddr, &iItem);
    return iItem - 1; //why -1 ? Oo

}
/*--------------------------------------------------------------------------*/
AssignedList * createAssignedList(void* _pvCtx, int _iRhs, int _iNbItem)
{
    AssignedList * newList = NULL;
    int iItem = 0;

    newList = (AssignedList*)MALLOC(sizeof(AssignedList));

    if (newList == NULL)
    {
        return NULL;
    }

    newList->iNbItem = _iNbItem + 1;
    newList->iCurItem = 2 ; /* begin with 1 and 1 are the names */
    newList->iRhs = _iRhs;

    /* get the stack pointer */
    getVarAddressFromPosition(_pvCtx, _iRhs, &newList->piList);
    getListItemNumber(_pvCtx, newList->piList, &iItem);

    /* check the size */
    if (iItem != newList->iNbItem)
    {
        FREE(newList);
        return NULL;
    }

    return newList;
}
/*--------------------------------------------------------------------------*/
void destroyAssignedList(AssignedList* _pList)
{
    FREE(_pList);
}
/*--------------------------------------------------------------------------*/
int getAssignedListNbElement(AssignedList* _pList)
{
    return _pList->iNbItem - 1;
}
/*--------------------------------------------------------------------------*/
void rewindAssignedList(AssignedList* _pList)
{
    _pList->iCurItem = 2; //why -2 ? Oo
}
/*--------------------------------------------------------------------------*/
BOOL isListCurrentElementDoubleMatrix(void* _pvCtx, AssignedList* _pList)
{
    int* piAddrItem = NULL;
    int iType = 0;
    getListItemAddress(_pvCtx, _pList->piList, _pList->iCurItem, &piAddrItem);
    getVarType(_pvCtx, piAddrItem, &iType);
    return iType == sci_matrix;
}
/*--------------------------------------------------------------------------*/
BOOL isListCurrentElementStringMatrix(void* _pvCtx, AssignedList* _pList)
{
    int* piAddrItem = NULL;
    int iType = 0;
    getListItemAddress(_pvCtx, _pList->piList, _pList->iCurItem, &piAddrItem);
    getVarType(_pvCtx, piAddrItem, &iType);
    return iType == sci_strings;
}
/*--------------------------------------------------------------------------*/
BOOL isListCurrentElementEmptyMatrix(void* _pvCtx, AssignedList* _pList)
{
    int* piItem = NULL;
    int iRows = 0, iCols = 0;

    if (!isListCurrentElementDoubleMatrix(_pvCtx, _pList))
    {
        /* empty matrix is a double matrix */
        return FALSE;
    }

    getListItemAddress(_pvCtx, _pList->piList, _pList->iCurItem, &piItem);
    getVarDimension(_pvCtx, piItem, &iRows, &iCols);
    if (iRows * iCols == 0)
    {
        return TRUE;
    }

    return FALSE;

}
/*--------------------------------------------------------------------------*/
double* getDoubleMatrixFromList(void* _pvCtx, AssignedList* _pList, int _iItem, int* _piRows, int* _piCols)
{
    double* pdbl = NULL;

    getMatrixOfDoubleInList(_pvCtx, _pList->piList, _iItem, _piRows, _piCols, &pdbl);
    return pdbl;
}
/*--------------------------------------------------------------------------*/
char ** getStringMatrixFromList(void* _pvCtx, AssignedList* _pList, int _iItem, int* _piRows, int* _piCols)
{
    int* piItem = NULL;
    char** pstData = NULL;
    getListItemAddress(_pvCtx, _pList->piList, _iItem, &piItem);
    if (getAllocatedMatrixOfString(_pvCtx, piItem, _piRows, _piCols, &pstData))
    {
        return NULL;
    }
    return pstData;
}
/*--------------------------------------------------------------------------*/
double* getCurrentDoubleMatrixFromList(void* _pvCtx, AssignedList* _pList, int* _piRows, int* _piCols)
{
    double* res = NULL;
    if (_pList->iCurItem > _pList->iNbItem)
    {
        *_piRows = 0;
        *_piCols = 0;
        return NULL;
    }

    res = getDoubleMatrixFromList(_pvCtx, _pList, _pList->iCurItem, _piRows, _piCols);
    _pList->iCurItem++;
    return res;
}
/*--------------------------------------------------------------------------*/
char** getCurrentStringMatrixFromList(void* _pvCtx, AssignedList* _pList, int* _piRows, int* _piCols)
{
    char** res = NULL;
    if (_pList->iCurItem > _pList->iNbItem)
    {
        *_piRows = 0;
        *_piCols = 0;
        return NULL;
    }

    res = getStringMatrixFromList(_pvCtx, _pList, _pList->iCurItem, _piRows, _piCols);
    _pList->iCurItem++;
    return res;

}
/*--------------------------------------------------------------------------*/
double* createCopyDoubleMatrixFromList(void* _pvCtx, AssignedList* _pList, int* _piRows, int* _piCols)
{
    /* get the matrix */
    double* stackValues = getCurrentDoubleMatrixFromList(_pvCtx, _pList, _piRows, _piCols);
    int nbElement = *_piRows * *_piCols;

    double* copyMatrix = NULL;

    if (nbElement == 0)
    {
        return NULL;
    }

    /* copy */

    copyMatrix = (double*)MALLOC((*_piRows) * (*_piCols) * sizeof(double));

    if (copyMatrix == NULL)
    {
        *_piRows = -1;
        *_piCols = -1;
        return NULL;
    }

    doubleArrayCopy(copyMatrix, stackValues, nbElement);

    return copyMatrix;

}
/*--------------------------------------------------------------------------*/
