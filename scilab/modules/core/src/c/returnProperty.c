/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* desc : a set of functions used to return values in Scilab              */
/*------------------------------------------------------------------------*/

#include "stack-c.h"
#include "returnProperty.h"
#include <string.h>

/*--------------------------------------------------------------------------*/
int sciReturnEmptyMatrix( void )
{
  int numRow   = 0 ;
  int numCol   = 0 ;
  int outIndex = 0 ;
  CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&numCol,&outIndex);
  return 0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnString( const char * value )
{
  int numRow   = 1 ;
  int numCol   = (int)strlen( value );
  int outIndex = 0 ;
  CreateVar(Rhs+1,STRING_DATATYPE,&numRow,&numCol,&outIndex);
  strncpy(cstk(outIndex),value, numCol);

  return  0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnChar( char value )
{
  int nbRow    = 1 ;
  int nbCol    = 1 ;
  int outIndex = 0 ;
  CreateVar(Rhs+1,STRING_DATATYPE,&nbRow,&nbCol,&outIndex);
  strncpy(cstk(outIndex), &value , 1 );

  return 0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnDouble( double value )
{
  int numRow   = 1 ;
  int numCol   = 1 ;
  int outIndex = 0 ;
  CreateVar( Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &numRow, &numCol, &outIndex );
  *stk(outIndex) = value ;

  return 0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnInt( int value )
{

  int numRow   = 1 ;
  int numCol   = 1 ;
  int outIndex = 0 ;
  CreateVar( Rhs+1, MATRIX_OF_INTEGER_DATATYPE, &numRow, &numCol, &outIndex );
  *istk(outIndex) = value ;

  return 0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnRowVector( const double values[], int nbValues )
{
  int numRow   = 1        ;
  int outIndex = 0        ;
  int i ;
  CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbValues,&outIndex);
  for ( i = 0 ; i < nbValues ; i++ )
  {
    stk(outIndex)[i] = values[i] ;
  }
  return 0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnRowVectorFromInt( const int values[], int nbValues )
{
  int numRow   = 1        ;
  int outIndex = 0        ;
  int i ;
  CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbValues,&outIndex);
  for ( i = 0 ; i < nbValues ; i++ )
  {
    stk(outIndex)[i] = (double)values[i] ;
  }
  return 0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnRowIntVector( const int values[], int nbValues )
{
  int numRow   = 1        ;
  int outIndex = 0        ;
  int i ;
  CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&numRow,&nbValues,&outIndex);
  for ( i = 0 ; i < nbValues ; i++ )
  {
    istk(outIndex)[i] = values[i] ;
  }
  return 0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnRowStringVector(char * values[], int nbValues )
{
  return sciReturnStringMatrix( values, 1, nbValues );
}
/*--------------------------------------------------------------------------*/
int sciReturnHandle( long handle )
{
  int numRow   = 1 ;
  int numCol   = 1 ;
  int outIndex = 0 ;
  CreateVar(Rhs+1,GRAPHICAL_HANDLE_DATATYPE,&numRow,&numCol,&outIndex);
  *hstk(outIndex) = handle ;
  return 0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnRowHandleVector( const long handles[], int nbValues )
{
  int numRow   = 1 ;
  int outIndex = 0 ;
  int i ;
  CreateVar( Rhs+1,GRAPHICAL_HANDLE_DATATYPE, &numRow, &nbValues, &outIndex );
  for ( i = 0 ; i < nbValues ; i++ )
  {
    hstk(outIndex)[i] = handles[i] ;
  }
  return 0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnColHandleVector( const long handles[], int nbValues )
{
  int numCol   = 1 ;
  int outIndex = 0 ;
  int i ;
  CreateVar( Rhs+1, GRAPHICAL_HANDLE_DATATYPE, &nbValues, &numCol, &outIndex );
  for ( i = 0 ; i < nbValues ; i++ )
  {
    hstk(outIndex)[i] = handles[i] ;
  }
  return 0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnMatrix( double values[], int nbRow, int nbCol )
{
  int outIndex = 0 ;
  int i ;
  CreateVar( Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &nbRow, &nbCol, &outIndex );
  for  ( i = 0; i < nbRow * nbCol; i++ )
  {
    stk(outIndex)[i] = values[i] ;
  }
  return 0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnStringMatrix(char * values[], int nbRow, int nbCol )
{
  CreateVarFromPtr( Rhs+1, MATRIX_OF_STRING_DATATYPE, &nbRow, &nbCol, values );
  return 0 ;
}
/*--------------------------------------------------------------------------*/
int sciReturnUserData( const int * userData, int userDataSize )
{
  int * data_ptr = NULL ;
  int data_size = (userDataSize + 1 ) / 2 ;
  SetWorkSize(Rhs+1,&data_size);
  data_ptr = GetRawData(Rhs+1);
  memcpy( data_ptr, userData, userDataSize * sizeof (int) );
  return 0 ;
}
/*--------------------------------------------------------------------------*/
