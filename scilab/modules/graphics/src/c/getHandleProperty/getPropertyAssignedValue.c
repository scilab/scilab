/*------------------------------------------------------------------------*/
/* file: getPropertyAssignedValue.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : a set of functions used to get the values which will be         */
/*        assigned to handles properties from the stack                   */
/*------------------------------------------------------------------------*/


#include "getPropertyAssignedValue.h"
#include "stack-c.h"
#include "sciprint.h"
#include "MALLOC.h"

/*-----------------------------------------------------------------------------------*/
BOOL isParameterHandle( int paramNum )
{
  return ( VarType( paramNum ) == 9 ) ;
}
/*-----------------------------------------------------------------------------------*/
BOOL isParameterDoubleMatrix( int paramNum )
{
  return ( VarType( paramNum ) == 1 ) ;
}
/*-----------------------------------------------------------------------------------*/
BOOL isParameterTlist( int paramNum )
{
  return ( VarType( paramNum ) == 16 ) ;
}
/*-----------------------------------------------------------------------------------*/
double getDoubleFromStack( int stackPointer )
{
  return *(stk( stackPointer ));
}
/*-----------------------------------------------------------------------------------*/
double * getDoubleMatrixFromStack( int stackPointer )
{
  return stk( stackPointer ) ;
}
/*-----------------------------------------------------------------------------------*/
void copyDoubleVectorFromStack( int stackPointer, double dest[], int nbElement )
{
  int i ;
  double * stackValue = getDoubleMatrixFromStack( stackPointer ) ;
  
  for ( i = 0 ; i < nbElement ; i++ )
  {
    dest[i] = stackValue[i] ;
  }
}
/*-----------------------------------------------------------------------------------*/
char * getStringFromStack( int stackPointer )
{
  return cstk( stackPointer ) ;
}
/*-----------------------------------------------------------------------------------*/
char ** getStringMatrixFromStack( int stackPointer )
{
  /* strange but it was taken from sci_set */
  return (char **) stackPointer ;
}
/*-----------------------------------------------------------------------------------*/
unsigned long getHandleFromStack( int stackPointer )
{
  return (unsigned long) *(hstk( stackPointer )) ;
}
/*-----------------------------------------------------------------------------------*/
BOOL isStringParamEqual( int stackPointer, const char * str )
{
  if ( strcmp( getStringFromStack( stackPointer ), str ) == 0 )
  {
    return TRUE ;
  }
  else
  {
    return FALSE ;
  }
  return FALSE ;
}
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
int getStackListNbElement( int paramNum )
{
  int nbRow        = 0 ;
  int nbCol        = 0 ;
  int stackPointer = 0 ;

  GetRhsVar( paramNum, "t", &nbRow, &nbCol, &stackPointer ) ;

  return nbRow - 1 ;

 }
/*-----------------------------------------------------------------------------------*/
AssignedList * createAssignedList( int paramNum, int nbElement )
{
  AssignedList * newList = NULL ;
  int nbRow = 0 ;
  int nbCol = 0 ;

  newList = MALLOC( sizeof(AssignedList) ) ;

  if ( newList == NULL )
  {
    return NULL ;
  }

  newList->nbElement   = nbElement + 1 ;
  newList->curElement  = 2 ; /* begin with 1 and 1 are the names */
  newList->paramNumber = paramNum ;

  /* get the stack pointer */
  GetRhsVar( paramNum, "t", &nbRow, &nbCol, &(newList->stackPointer) ) ;

  /* check the size */
  if ( nbRow != newList->nbElement || nbCol != 1 )
  {
    sciprint( "Wrong size for tlist.\n" ) ;
    return NULL ;
  }
  return newList ;
}
/*-----------------------------------------------------------------------------------*/
void destroyAssignedList( AssignedList * list )
{
  FREE( list ) ;
}
/*-----------------------------------------------------------------------------------*/
int getAssignedListNbElement( AssignedList * list )
{
  return list->nbElement - 1 ;
}
/*-----------------------------------------------------------------------------------*/
void rewindAssingnedList( AssignedList * list )
{
  list->curElement = 2 ;
}
/*-----------------------------------------------------------------------------------*/
double * getDoubleMatrixFromList( AssignedList * list, int rank, int * nbRow, int * nbCol )
{
  int valueStackPointer = 0 ;
  GetListRhsVar( list->paramNumber, rank, "d", nbRow, nbCol, &valueStackPointer ) ;
  
  return getDoubleMatrixFromStack( valueStackPointer ) ;
}
/*-----------------------------------------------------------------------------------*/
double * getCurrentDoubleMatrixFromList( AssignedList * list, int * nbRow, int * nbCol )
{
  double * res = NULL ;
  if ( list->curElement > list->nbElement )
  {
    *nbRow = 0 ;
    *nbCol = 0 ;
    return NULL ;
  }

  res = getDoubleMatrixFromList( list, list->curElement, nbRow, nbCol ) ;
  list->curElement++ ;
  return res ;
}
/*-----------------------------------------------------------------------------------*/
double * createCopyDoubleMatrixFromList( AssignedList * list, int * nbRow, int * nbCol )
{
  int i ;
  /* get the matrix */
  double * stackValues = getCurrentDoubleMatrixFromList( list, nbRow, nbCol ) ;

  /* copy */
  double * copyMatrix = MALLOC( (*nbRow) * (*nbCol) * sizeof( double ) ) ;

  if ( copyMatrix == NULL )
  {
    return NULL ;
  }

  for ( i = 0 ; i < (*nbRow) * (*nbCol) ; i++ )
  {
    copyMatrix[i] = stackValues[i] ;
  }

  return copyMatrix ;

}
/*-----------------------------------------------------------------------------------*/