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
#include "BasicAlgos.h"

/*-----------------------------------------------------------------------------------*/
BOOL isParameterHandle( int type )
{
  return ( type == 9 ) ;
}
/*-----------------------------------------------------------------------------------*/
BOOL isParameterDoubleMatrix( int type )
{
  return ( type == 1 ) ;
}
/*-----------------------------------------------------------------------------------*/
BOOL isParameterTlist( int type )
{
  return ( type == 16 ) ;
}
/*-----------------------------------------------------------------------------------*/
BOOL isParameterStringMatrix( int type )
{
  return ( type == 10 ) ;
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
  doubleArrayCopy( dest, getDoubleMatrixFromStack( stackPointer ), nbElement ) ;
}
/*-----------------------------------------------------------------------------------*/
void copyDoubleVectorToIntFromStack( int stackPointer, int dest[], int nbElement )
{ 
  int i ;
  double * values = getDoubleMatrixFromStack( stackPointer ) ;
  for ( i = 0 ; i < nbElement ; i++ )
  {
    dest[i] = (int) values[i] ;
  }
}
/*-----------------------------------------------------------------------------------*/
double * createCopyDoubleVectorFromStack( int stackPointer, int nbElement )
{
  double * res = MALLOC( nbElement * sizeof(double) ) ;
  if ( res == NULL )
  {
    return NULL ;
  }
  copyDoubleVectorFromStack( stackPointer, res, nbElement ) ;
  return res ;
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
char ** createCopyStringMatrixFromStack( int stackPointer, int nbElement )
{
  int i ;
  char ** res    = MALLOC( nbElement * sizeof(char *) ) ;
  char ** values = getStringMatrixFromStack( stackPointer ) ;
  
  if ( res == NULL )
  {
    return NULL ;
  }

  for ( i = 0 ; i < nbElement ; i++ )
  {
    int size =  (int)strlen( values[i] ) + 1 ;
    res[i] = MALLOC( size * sizeof(char) ) ;

    if ( res[i] == NULL )
    {
      /* deallocate what have been allocated */
      int j ;
      for ( j = 0 ; j < i ; j++ )
      {
        FREE( res[j] ) ;
      }
      FREE( res ) ;
      return NULL ;
    }

    strcpy( res[i], values[i] ) ;
  }

  return res ;

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
void rewindAssignedList( AssignedList * list )
{
  list->curElement = 2 ;
}
/*-----------------------------------------------------------------------------------*/
BOOL isListCurrentElementDoubleMatrix( AssignedList * list )
{
  return ( ElementType( list->paramNumber, list->curElement ) == 1 ) ;
}
/*-----------------------------------------------------------------------------------*/
BOOL isListCurrentElementStringMatrix( AssignedList * list )
{
  return ( ElementType( list->paramNumber, list->curElement ) == 10 ) ;
}
/*-----------------------------------------------------------------------------------*/
BOOL isListCurrentElementEmptyMatrix( AssignedList * list )
{
  int nbRow = 0 ;
  int nbCol = 0 ;

  if ( !isListCurrentElementDoubleMatrix( list ) )
  {
    /* empty matrix is a double matrix */
    return FALSE ;
  }

  getDoubleMatrixFromList( list, list->curElement, &nbRow, &nbCol ) ;

  if ( nbRow * nbCol == 0 )
  {
    return TRUE ;
  }

  return FALSE ;

}
/*-----------------------------------------------------------------------------------*/
double * getDoubleMatrixFromList( AssignedList * list, int rank, int * nbRow, int * nbCol )
{
  int valueStackPointer = 0 ;
  GetListRhsVar( list->paramNumber, rank, "d", nbRow, nbCol, &valueStackPointer ) ;
  
  return getDoubleMatrixFromStack( valueStackPointer ) ;
}
/*-----------------------------------------------------------------------------------*/
char ** getStringMatrixFromList( AssignedList * list, int rank, int * nbRow, int * nbCol )
{
  int valueStackPointer = 0 ;
  GetListRhsVar( list->paramNumber, rank, "S", nbRow, nbCol, &valueStackPointer ) ;

  return getStringMatrixFromStack( valueStackPointer ) ;
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
char ** getCurrentStringMatrixFromList( AssignedList * list, int * nbRow, int * nbCol )
{
  char ** res = NULL ;
  if ( list->curElement > list->nbElement )
  {
    *nbRow = 0 ;
    *nbCol = 0 ;
    return NULL ;
  }

  res = getStringMatrixFromList( list, list->curElement, nbRow, nbCol ) ;
  list->curElement++ ;
  return res ;

}
/*-----------------------------------------------------------------------------------*/
double * createCopyDoubleMatrixFromList( AssignedList * list, int * nbRow, int * nbCol )
{
  /* get the matrix */
  double * stackValues = getCurrentDoubleMatrixFromList( list, nbRow, nbCol ) ;
  int nbElement = (*nbRow) * (*nbCol) ;

  double * copyMatrix = NULL ;

  if ( nbElement == 0 )
  {
    return NULL ;
  }

  /* copy */

  copyMatrix = MALLOC( (*nbRow) * (*nbCol) * sizeof( double ) ) ;

  if ( copyMatrix == NULL )
  { 
    *nbRow = -1 ;
    *nbCol = -1 ;
    return NULL ;
  }

  doubleArrayCopy( copyMatrix, stackValues, nbElement ) ;

  return copyMatrix ;

}
/*-----------------------------------------------------------------------------------*/
char ** createCopyStringMatrixFromList( AssignedList * list, int * nbRow, int * nbCol )
{
  /* get the matrix */
  char ** stackValues = getCurrentStringMatrixFromList( list, nbRow, nbCol ) ;
  int nbElement = (*nbRow) * (*nbCol) ;

  if ( nbElement == 0 )
  {
    return NULL ;
  }

  return createStringArrayCopy( stackValues, nbElement ) ;
}
/*-----------------------------------------------------------------------------------*/
