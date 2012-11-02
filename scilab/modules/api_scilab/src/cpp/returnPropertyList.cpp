/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste SILVY
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*------------------------------------------------------------------------*/
/* file: returnProperty.c                                                 */
/* desc : a set of functions used to return a tList in Scilab             */
/*------------------------------------------------------------------------*/

#include "returnPropertyList.h"
#include "returnProperty.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "localization.h"
#include "tlist.hxx"
#include "double.hxx"
#include "string.hxx"
/*------------------------------------------------------------------------*/
void* createReturnedList(int nbElements, char * elementName[])
{
  //returnedList * newList = NULL ;
  //int nbRow = 1 ;

  //newList = (returnedList*)MALLOC(sizeof(returnedList)) ;
  //if (newList == NULL)
  //{
  //  sciprint(_("Error returning tlist, memory full.\n")) ;
  //  return NULL ;
  //}

  //newList->nbElements = nbElements + 1 ;

  ///* create the tlist in the stack and get the stack pointer in newList->stackPointer */
  //CreateVar(Rhs+1,TYPED_LIST_DATATYPE, &(newList->nbElements), &nbRow, &(newList->stackPointer));

  ///* create the first element : names of properties */
  //CreateListVarFromPtr(Rhs+1, 1,MATRIX_OF_STRING_DATATYPE, &nbRow, &(newList->nbElements), elementName);
  //
  //newList->curElement = 1 ;

  //return newList ;

    //create fieldname list
    types::String* pField = (types::String*)sciReturnStringMatrix(elementName, 1, nbElements + 1);
    //create TList
    types::TList* pL = new types::TList();
    //add field list in list
    pL->append(pField);
    return pL;

}
/*------------------------------------------------------------------------*/
int destroyReturnedList(void* list)
{
  /* safe test to know if the list has been correctly filled */
  //if (list->curElement != list->nbElements)
  //{
  //  sciprint(_("tlist not completely filled.\n")) ;
  //  FREE(list) ;
  //  return 1 ;
  //}

  //FREE(list) ;
  //return 0 ;

    delete (types::TList*)list;
    return 0;
}
/*------------------------------------------------------------------------*/
int addRowVectorToReturnedList(void* list, const double vector[], int nbValues)
{
  //int nbRow = 1 ;
  ///* check we are not using all the allocated space for the list */
  //if (list->curElement >= list->nbElements)
  //{
  //  sciprint(_("List full.\n")) ;
  //  return 1 ;
  //}

  ///* add a new element */
  //list->curElement++ ;
  //CreateListVarFromPtr(Rhs+1, list->curElement,MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbValues, &vector);
  //return 0 ;

    types::TList* pL = (types::TList*)list;
    pL->append((types::InternalType*)sciReturnMatrix((double*)vector, nbValues, 1));
    return 0;
}
/*------------------------------------------------------------------------*/
int addColVectorToReturnedList(void* list, const double vector[], int nbValues)
{
  //int nbCol = 1 ;
  ///* check we are not using all the allocated space for the list */
  //if (list->curElement >= list->nbElements)
  //{
  //  sciprint(_("List full.\n")) ;
  //  return 1 ;
  //}

  ///* add a new element */
  //list->curElement++ ;
  //CreateListVarFromPtr(Rhs+1, list->curElement,MATRIX_OF_DOUBLE_DATATYPE, &nbValues, &nbCol, &vector);

  //return 0 ;

    types::TList* pL = (types::TList*)list;
    pL->append((types::InternalType*)sciReturnMatrix((double*)vector, nbValues, 1));
    return 0;
}
/*------------------------------------------------------------------------*/
int addMatrixToReturnedList(void* list, const double matrix[], int nbRow, int nbCol)
{
  ///* check we are not using all the allocated space for the list */
  //if (list->curElement >= list->nbElements)
  //{
  //  sciprint(_("List full.\n")) ;
  //  return 1 ;
  //}

  ///* add a new element */
  //list->curElement++ ;
  //CreateListVarFromPtr(Rhs+1, list->curElement,MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &matrix);

  //return 0 ;

    types::TList* pL = (types::TList*)list;
    pL->append((types::InternalType*)sciReturnMatrix((double*)matrix, nbRow, nbCol));
    return 0;
}
/*------------------------------------------------------------------------*/
int addStringColVectorToReturnedList(void* list, char * vector[], int nbValues)
{
  //int nbCol = 1 ;
  ///* check we are not using all the allocated space for the list */
  //if (list->curElement >= list->nbElements)
  //{
  //  sciprint(_("List full.\n")) ;
  //  return 1 ;
  //}

  ///* add a new element */
  //list->curElement++ ;
  //CreateListVarFromPtr(Rhs+1, list->curElement,MATRIX_OF_STRING_DATATYPE, &nbValues, &nbCol, vector);

  //return 0 ;

    types::TList* pL = (types::TList*)list;
    pL->append((types::InternalType*)sciReturnStringMatrix(vector, nbValues, 1));
    return 0;
}
/*------------------------------------------------------------------------*/
