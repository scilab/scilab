/*------------------------------------------------------------------------*/
/* file: returnProperty.c                                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : a set of functions used to return values in Scilab              */
/*------------------------------------------------------------------------*/

#include "stack-c.h"
#include "returnProperty.h"
#include <string.h>

/*-----------------------------------------------------------------------------------*/
int sciReturnEmptyMatrix( void )
{
  int numRow   = 0 ;
  int numCol   = 0 ;
  int outIndex = 0 ;
  CreateVar(Rhs+1,"d",&numRow,&numCol,&outIndex) ;
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnString( const char * value )
{
  int numRow   = 1 ;
  int numCol   = strlen( value ) ;
  int outIndex = 0 ;
  CreateVar(Rhs+1,"c",&numRow,&numCol,&outIndex);
  strncpy(cstk(outIndex),value, numCol);

  return  0 ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnDouble( double value )
{
  int numRow   = 1 ;
  int numCol   = 1 ;
  int outIndex = 0 ;
  CreateVar( Rhs+1, "d", &numRow, &numCol, &outIndex ) ;
  *stk(outIndex) = value ;

  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnInt( int value )
{

  int numRow   = 1 ;
  int numCol   = 1 ;
  int outIndex = 0 ;
  CreateVar( Rhs+1, "i", &numRow, &numCol, &outIndex ) ;
  *istk(outIndex) = value ;

  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnRowVector( const double values[], int nbValues )
{
  int numRow   = 1        ;
  int outIndex = 0        ;
  int i ;
  CreateVar(Rhs+1,"d",&numRow,&nbValues,&outIndex) ;
  for ( i = 0 ; i < nbValues ; i++ )
  {
    stk(outIndex)[i] = values[i] ;
  }
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnRowVectorFromInt( const int values[], int nbValues )
{
  int numRow   = 1        ;
  int outIndex = 0        ;
  int i ;
  CreateVar(Rhs+1,"d",&numRow,&nbValues,&outIndex) ;
  for ( i = 0 ; i < nbValues ; i++ )
  {
    stk(outIndex)[i] = (double)values[i] ;
  }
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnRowIntVector( const int values[], int nbValues )
{
  int numRow   = 1        ;
  int outIndex = 0        ;
  int i ;
  CreateVar(Rhs+1,"i",&numRow,&nbValues,&outIndex) ;
  for ( i = 0 ; i < nbValues ; i++ )
  {
    istk(outIndex)[i] = values[i] ;
  }
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnRowStringVector( const char * values[], int nbValues )
{
  return sciReturnStringMatrix( values, 1, nbValues ) ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnHandle( long handle )
{
  int numRow   = 1 ;
  int numCol   = 1 ;
  int outIndex = 0 ;
  CreateVar(Rhs+1,"h",&numRow,&numCol,&outIndex);
  *hstk(outIndex) = handle ;
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnRowHandleVector( const long handles[], int nbValues )
{
  int numRow   = 1 ;
  int outIndex = 0 ;
  int i ;
  CreateVar( Rhs+1, "h", &numRow, &nbValues, &outIndex ) ;
  for ( i = 0 ; i < nbValues ; i++ )
  {
    hstk(outIndex)[i] = handles[i] ;
  }
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnMatrix( const double values[], int nbRow, int nbCol )
{
  int outIndex = 0 ;
  int i ;
  CreateVar( Rhs+1, "d", &nbRow, &nbCol, &outIndex ) ;
  for  ( i = 0; i < nbRow * nbCol; i++ )
  {
    stk(outIndex)[i] = values[i] ;
  }
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
int sciReturnStringMatrix( const char * values[], int nbRow, int nbCol )
{
  CreateVarFromPtr( Rhs+1, "S", &nbRow, &nbCol, values ) ;
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
