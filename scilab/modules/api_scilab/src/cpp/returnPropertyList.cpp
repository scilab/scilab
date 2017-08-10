/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste SILVY
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
/* file: returnProperty.c                                                 */
/* desc : a set of functions used to return a tList in Scilab             */
/*------------------------------------------------------------------------*/



#include "returnPropertyList.h"
#include "returnProperty.h"
#include "sci_malloc.h"
#include "sciprint.h"
#include "localization.h"
#include "tlist.hxx"
#include "double.hxx"
#include "string.hxx"
/*------------------------------------------------------------------------*/
void* createReturnedList(int nbElements, char * elementName[])
{
    //create fieldname list
    types::String* pField = (types::String*)sciReturnStringMatrix(elementName, 1, nbElements + 1);
    //create TList
    types::TList* pL = new types::TList();
    //add field list in list
    pL->append(pField);
    return pL;
}
/*------------------------------------------------------------------------*/
int addRowVectorToReturnedList(void* list, const double vector[], int nbValues)
{
    types::TList* pL = (types::TList*)list;
    pL->append((types::InternalType*)sciReturnMatrix((double*)vector, 1, nbValues));
    return 0;
}
/*------------------------------------------------------------------------*/
int addColVectorToReturnedList(void* list, const double vector[], int nbValues)
{
    types::TList* pL = (types::TList*)list;
    pL->append((types::InternalType*)sciReturnMatrix((double*)vector, nbValues, 1));
    return 0;
}
/*------------------------------------------------------------------------*/
int addMatrixToReturnedList(void* list, const double matrix[], int nbRow, int nbCol)
{
    types::TList* pL = (types::TList*)list;
    pL->append((types::InternalType*)sciReturnMatrix((double*)matrix, nbRow, nbCol));
    return 0;
}
/*------------------------------------------------------------------------*/
int addStringColVectorToReturnedList(void* list, char * vector[], int nbValues)
{
    types::TList* pL = (types::TList*)list;
    pL->append((types::InternalType*)sciReturnStringMatrix(vector, nbValues, 1));
    return 0;
}
/*------------------------------------------------------------------------*/
int addStringToReturnedList(void* list, char * str)
{
    types::TList* pL = (types::TList*)list;
    pL->append((types::InternalType*)sciReturnString(str));
    return 0 ;
}
