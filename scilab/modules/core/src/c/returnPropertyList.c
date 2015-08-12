/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste SILVY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*------------------------------------------------------------------------*/
/* file: returnProperty.c                                                 */
/* desc : a set of functions used to return a tList in Scilab             */
/*------------------------------------------------------------------------*/

#include "returnPropertyList.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "localization.h"
/*------------------------------------------------------------------------*/
returnedList * createReturnedList( int nbElements, char * elementName[] )
{
    returnedList * newList = NULL ;
    int nbRow = 1 ;

    newList = MALLOC( sizeof(returnedList) ) ;
    if ( newList == NULL )
    {
        sciprint(_("Error returning tlist, memory full.\n")) ;
        return NULL ;
    }

    newList->nbElements = nbElements + 1 ;

    /* create the tlist in the stack and get the stack pointer in newList->stackPointer */
    CreateVar( Rhs + 1, TYPED_LIST_DATATYPE, &(newList->nbElements), &nbRow, &(newList->stackPointer) );

    /* create the first element : names of properties */
    CreateListVarFromPtr( Rhs + 1, 1, MATRIX_OF_STRING_DATATYPE, &nbRow, &(newList->nbElements), elementName );

    newList->curElement = 1 ;

    return newList ;
}
/*------------------------------------------------------------------------*/
int destroyReturnedList( returnedList * list )
{
    /* safe test to know if the list has been correctly filled */
    if ( list->curElement != list->nbElements )
    {
        sciprint( _("tlist not completely filled.\n") ) ;
        FREE( list ) ;
        return 1 ;
    }

    FREE( list ) ;
    return 0 ;
}
/*------------------------------------------------------------------------*/
int addRowVectorToReturnedList( returnedList * list, const double vector[], int nbValues )
{
    int nbRow = 1 ;
    /* check we are not using all the allocated space for the list */
    if ( list->curElement >= list->nbElements )
    {
        sciprint(_("List full.\n")) ;
        return 1 ;
    }

    /* add a new element */
    list->curElement++ ;
    CreateListVarFromPtr( Rhs + 1, list->curElement, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbValues, &vector );

    return 0 ;
}
/*------------------------------------------------------------------------*/
int addColVectorToReturnedList( returnedList * list, const double vector[], int nbValues )
{
    int nbCol = 1 ;
    /* check we are not using all the allocated space for the list */
    if ( list->curElement >= list->nbElements )
    {
        sciprint(_("List full.\n")) ;
        return 1 ;
    }

    /* add a new element */
    list->curElement++ ;
    CreateListVarFromPtr( Rhs + 1, list->curElement, MATRIX_OF_DOUBLE_DATATYPE, &nbValues, &nbCol, &vector );

    return 0 ;
}
/*------------------------------------------------------------------------*/
int addMatrixToReturnedList( returnedList * list, const double matrix[], int nbRow, int nbCol )
{
    /* check we are not using all the allocated space for the list */
    if ( list->curElement >= list->nbElements )
    {
        sciprint(_("List full.\n")) ;
        return 1 ;
    }

    /* add a new element */
    list->curElement++ ;
    CreateListVarFromPtr( Rhs + 1, list->curElement, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &matrix );

    return 0 ;
}
/*------------------------------------------------------------------------*/
int addStringColVectorToReturnedList( returnedList * list, char * vector[], int nbValues )
{
    int nbCol = 1 ;
    /* check we are not using all the allocated space for the list */
    if ( list->curElement >= list->nbElements )
    {
        sciprint(_("List full.\n")) ;
        return 1 ;
    }

    /* add a new element */
    list->curElement++ ;
    CreateListVarFromPtr( Rhs + 1, list->curElement, MATRIX_OF_STRING_DATATYPE, &nbValues, &nbCol, vector );

    return 0 ;
}
/*------------------------------------------------------------------------*/
int addStringToReturnedList( returnedList * list, char * str)
{
    int nbValues = 1;
    int nbCol = 1 ;
    /* check we are not using all the allocated space for the list */
    if ( list->curElement >= list->nbElements )
    {
        sciprint(_("List full.\n")) ;
        return 1 ;
    }

    /* add a new element */
    list->curElement++ ;
    CreateListVarFromPtr( Rhs + 1, list->curElement, MATRIX_OF_STRING_DATATYPE, &nbValues, &nbCol, &str);

    return 0 ;
}