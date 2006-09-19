/*------------------------------------------------------------------------*/
/* file: returnProperty.c                                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : a set of functions used to return a tList in Scilab             */
/*------------------------------------------------------------------------*/

#include "returnPropertyList.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
returnedList * createNewReturnedList( int nbElement, const char * elementName[] )
{
  returnedList * newList = NULL ;
  int nbRow = 1 ;

  newList = MALLOC( sizeof(returnedList) ) ;
  if ( newList == NULL )
  {
    sciprint("Error returning tlist, memory full.\n") ;
    return NULL ;
  }

  newList->curElement = 0 ;

  /* create the tlist in the stack and get the stack pointer in newList->stackPointer */
  CreateVar( Rhs+1, "t", &nbRow, &nbElement, &(newList->stackPointer) ) ;

  /* create the first element : names of properties */
  CreateListVarFromPtr( Rhs+1, 1, "S", &nbRow, &nbElement, elementName ) ;
  
  newList->curElement = 1 ;

  return newList ;
}
/*------------------------------------------------------------------------*/
int destroyReturnedList( returnedList * list )
{
  if ( list->curElement < list->nbElements )
  {
    sciprint( "tList not completely filled.\n" ) ;
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
  if ( list->curElement >= list->nbElements )
  {
    sciprint("list full.\n") ;
    return 1 ;
  }
  list->curElement++ ;
  CreateListVarFromPtr( Rhs+1, list->curElement, "d", &nbRow, &nbValues, &vector ) ;
  return 0 ;
}
/*------------------------------------------------------------------------*/
int addColVectorToReturnedList( returnedList * list, const double vector[], int nbValues )
{
  int nbCol = 1 ;
  if ( list->curElement >= list->nbElements )
  {
    sciprint("list full.\n") ;
    return 1 ;
  }
  list->curElement++ ;
  CreateListVarFromPtr( Rhs+1, list->curElement, "d", &nbValues, &nbCol, &vector ) ;
  return 0 ;
}
/*------------------------------------------------------------------------*/
int addMatrixToReturnedList( returnedList * list, const double matrix[], int nbRow, int nbCol )
{
  if ( list->curElement >= list->nbElements )
  {
    sciprint("list full.\n") ;
    return 1 ;
  }
  list->curElement++ ;
  CreateListVarFromPtr( Rhs+1, list->curElement, "d", &nbRow, &nbCol, &matrix ) ;
  return 0 ;
}
/*------------------------------------------------------------------------*/
