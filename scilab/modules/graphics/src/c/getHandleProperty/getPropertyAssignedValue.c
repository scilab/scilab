/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: getPropertyAssignedValue.c                                       */
/* desc : a set of functions used to get the values which will be         */
/*        assigned to handles properties from the stack                   */
/*------------------------------------------------------------------------*/


#include "getPropertyAssignedValue.h"
#include "stack-c.h"
#include "localization.h"
#include "MALLOC.h"
#include "BasicAlgos.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
BOOL isParameterHandle( int type )
{
  return ( type == sci_handles ) ;
}
/*--------------------------------------------------------------------------*/
BOOL isParameterDoubleMatrix( int type )
{
  return ( type == sci_matrix ) ;
}
/*--------------------------------------------------------------------------*/
BOOL isParameterTlist( int type )
{
  return ( type == sci_tlist ) ;
}
/*--------------------------------------------------------------------------*/
BOOL isParameterStringMatrix( int type )
{
  return ( type == sci_strings ) ;
}
/*--------------------------------------------------------------------------*/
double getDoubleFromStack( size_t stackPointer )
{
  return *(stk( stackPointer ));
}
/*--------------------------------------------------------------------------*/
double * getDoubleMatrixFromStack( size_t stackPointer )
{
  return stk( stackPointer ) ;
}
/*--------------------------------------------------------------------------*/
void copyDoubleVectorFromStack( size_t stackPointer, double dest[], int nbElement )
{ 
  doubleArrayCopy( dest, getDoubleMatrixFromStack( stackPointer ), nbElement ) ;
}
/*--------------------------------------------------------------------------*/
void copyDoubleVectorToIntFromStack( size_t stackPointer, int dest[], int nbElement )
{ 
  int i ;
  double * values = getDoubleMatrixFromStack( stackPointer ) ;
  for ( i = 0 ; i < nbElement ; i++ )
  {
    dest[i] = (int) values[i] ;
  }
}
/*--------------------------------------------------------------------------*/
double * createCopyDoubleVectorFromStack( size_t stackPointer, int nbElement )
{
  double * res = MALLOC( nbElement * sizeof(double) ) ;
  if ( res == NULL )
  {
    return NULL ;
  }
  copyDoubleVectorFromStack( stackPointer, res, nbElement ) ;
  return res ;
}
/*--------------------------------------------------------------------------*/
char * getStringFromStack( size_t stackPointer )
{
  return cstk( stackPointer ) ;
}
/*--------------------------------------------------------------------------*/
char ** getStringMatrixFromStack( size_t stackPointer )
{
  /* strange but it was taken from sci_set */
  return (char **) stackPointer ;
}
/*--------------------------------------------------------------------------*/
char ** createCopyStringMatrixFromStack( size_t stackPointer, int nbElement )
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
		freeArrayOfString(res, i);
		return NULL ;
    }

    strcpy( res[i], values[i] ) ;
  }

  return res ;

}
/*--------------------------------------------------------------------------*/
unsigned long getHandleFromStack( size_t stackPointer )
{
  return (unsigned long) *(hstk( stackPointer )) ;
}
/*--------------------------------------------------------------------------*/
BOOL isStringParamEqual( size_t stackPointer, const char * str )
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
/*--------------------------------------------------------------------------*/
int tryGetBooleanValueFromStack(size_t stackPointer, int valueType, int nbRow, int nbCol, char* propertyName)
{
  if(valueType == sci_strings)
  {
    if(isStringParamEqual(stackPointer, "on"))  return TRUE;
    if(isStringParamEqual(stackPointer, "off")) return FALSE;
    if(isStringParamEqual(stackPointer, "1"))   return TRUE;
    if(isStringParamEqual(stackPointer, "0"))   return FALSE;
    if(isStringParamEqual(stackPointer, "T"))   return TRUE;
    if(isStringParamEqual(stackPointer, "F"))   return FALSE;

    Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), propertyName, "on", "off");
    return NOT_A_BOOLEAN_VALUE;
  }

  if(valueType == sci_boolean)
  {
    return (int) *istk(stackPointer);
  }

  if(valueType == sci_matrix)
  {
    if(getDoubleFromStack(stackPointer) == 0) return FALSE;
    return TRUE;    
  }

  Scierror(999, _("Wrong type for '%s' property: String expected.\n"), propertyName);
  return NOT_A_BOOLEAN_VALUE;
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/
int getStackListNbElement( int paramNum )
{
  int nbRow        = 0 ;
  int nbCol        = 0 ;
  size_t stackPointer = 0 ;

  GetRhsVar( paramNum,TYPED_LIST_DATATYPE, &nbRow, &nbCol, &stackPointer );

  return nbRow - 1 ;

 }
/*--------------------------------------------------------------------------*/
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
  GetRhsVar( paramNum,TYPED_LIST_DATATYPE, &nbRow, &nbCol, &(newList->stackPointer) );

  /* check the size */
  if ( nbRow != newList->nbElement || nbCol != 1 )
  {
    return NULL ;
  }
  return newList ;
}
/*--------------------------------------------------------------------------*/
void destroyAssignedList( AssignedList * list )
{
  FREE( list ) ;
}
/*--------------------------------------------------------------------------*/
int getAssignedListNbElement( AssignedList * list )
{
  return list->nbElement - 1 ;
}
/*--------------------------------------------------------------------------*/
void rewindAssignedList( AssignedList * list )
{
  list->curElement = 2 ;
}
/*--------------------------------------------------------------------------*/
BOOL isListCurrentElementDoubleMatrix( AssignedList * list )
{
  return ( ElementType( list->paramNumber, list->curElement ) == 1 ) ;
}
/*--------------------------------------------------------------------------*/
BOOL isListCurrentElementStringMatrix( AssignedList * list )
{
  return ( ElementType( list->paramNumber, list->curElement ) == 10 ) ;
}
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
double * getDoubleMatrixFromList( AssignedList * list, int rank, int * nbRow, int * nbCol )
{
  int valueStackPointer = 0 ;
  GetListRhsVar( list->paramNumber, rank,MATRIX_OF_DOUBLE_DATATYPE, nbRow, nbCol, &valueStackPointer );
  
  return getDoubleMatrixFromStack( valueStackPointer ) ;
}
/*--------------------------------------------------------------------------*/
char ** getStringMatrixFromList( AssignedList * list, int rank, int * nbRow, int * nbCol )
{
  size_t valueStackPointer = 0 ;
  GetListRhsVar( list->paramNumber, rank,MATRIX_OF_STRING_DATATYPE, nbRow, nbCol, &valueStackPointer );

  return getStringMatrixFromStack( valueStackPointer ) ;
}
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
char ** createCopyStringMatrixFromList( AssignedList * list, int * nbRow, int * nbCol )
{
  char ** stackValues = NULL; 
  int nbElement = (*nbRow) * (*nbCol) ;

  if ( nbElement == 0 )
  {
    return NULL ;
  }

  /* get the matrix */
  stackValues = getCurrentStringMatrixFromList( list, nbRow, nbCol ) ;

  return createStringArrayCopy( stackValues, nbElement ) ;
}
/*--------------------------------------------------------------------------*/
